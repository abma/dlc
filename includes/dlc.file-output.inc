<?php
/* This file is part of Springfiles (GPL v2 or later), see LICENSE.txt */

/**
 * @author abma <spam@abma.de>
 * @file
 * used for the cron job
 */

/**
 *  generates some statistics blocks/pages for the file pages
 */

function dlc_nodeapi(&$node, $op, $teaser) {
  $result = array();
  switch ($op) {
    case 'view' :
      
      //get all filefield files
      if (module_exists('filefield'))
      {
        foreach (filefield_get_field_list($node->type) as $fieldname => $filefield_data)
        {
          foreach($node->$fieldname as &$filefield)
          {
            //attach mirror data to the filefield array
            dsm($filefield);
            $downloads = db_fetch_object(db_query("SELECT count FROM {dlc_file} WHERE fid = %d", $filefield['fid']));
            $filefield['downloads'] = $downloads->count;
          }
        }
      }
      break;
  }
}

/**
* Implementation of hook_block().
*/
function dlc_block($op = 'list', $delta = 0, $edit = array()) {
   $block = array();
   switch ($op) {
    // add our blocks to the block configuration screen
    case 'list':
        $block[0] = array(
          'info' => t('DLC file daily downloads'), 
        );
        $block[1] = array(
          'info' => t('DLC file weekly downloads'), 
        );
        $block[2] = array(
          'info' => t('DLC file monthly downloads'), 
        );
        $block[3] = array(
          'info' => t('DLC file yearly downloads'), 
        );
        return $block;
    case 'view':
        switch ($delta) {
          case 0:
            // Load the current node from url to get the fid
            $fid = node_load(arg(1))->field_file[0]['fid'];
            if (strlen($fid)) {
                $block = array(
                  'subject' => t('Daily Downloads'), 
                  'content' => dlc_file_stats_day(10, $fid),
                );
            }
            break;
        case 1:
           // Load the current node from url to get the fid
            $fid = node_load(arg(1))->field_file[0]['fid'];
            if (strlen($fid)) {
                $block = array(
                  'subject' => t('Weekly Downloads'), 
                  'content' => dlc_file_stats_week(10, $fid),
                );
            }
            break;
        case 2:
            // Load the current node from url to get the fid
            $fid = node_load(arg(1))->field_file[0]['fid'];
            if (strlen($fid)) {
                $block = array(
                  'subject' => t('Monthly Downloads'), 
                  'content' => dlc_file_stats_month(10, $fid),
                );
            }
            break;
        case 3:
            // Load the current node from url to get the fid
            $fid = node_load(arg(1))->field_file[0]['fid'];
            if (strlen($fid)) {
                $block = array(
                  'subject' => t('Yearly Downloads'), 
                  'content' => dlc_file_stats_year(10, $fid),
                );
            }
            break;
        }
        return $block;
    }
}
/**
* generated daily stats output.
* arguments: @amount = the number of days to show
*            @fid = the file id to base the stats on
*/
function dlc_file_stats_day($amount = 10, $fid) {
   $date=time();
   $date = $date - ($date % 86400);
   $end = $date - $amount * 86400;
   
   $result = db_query("select * from {dlc_file_day} where day<=%d AND day>=%d AND fid = %d ORDER BY day DESC", $date, $end, $fid );
	$arr=array();
	$max=0;
	while ($row = db_fetch_array($result)) {
			if ($max<$row['count'])
					$max=$row['count'];
			$arr[]=$row;
	}
	foreach($arr as $row){
		$output.= "<table><tr>";
		$output.= "<td>".strftime("%Y.%m.%d",$row['day'])."</td>";
		$output.= "<td>".create_bar($row['count'], $max, $row['count'], 128 )."</td>";
		$output.= "</tr></table>";
	}
	return $output;
}

/**
* generated weekly stats output.
* arguments: @amount = the number of days to show
*            @fid = the file id to base the stats on
*/
function dlc_file_stats_week($amount = 10, $fid) {
    $result = db_query("select * from {dlc_file_week} WHERE fid = %d ORDER BY week DESC LIMIT 0, %d ", array($fid, 10));
    $arr=array();
    $max=0;
    while ($row = db_fetch_array($result)) {
                    if ($max<$row['count'])
                                    $max=$row['count'];
                    $arr[]=$row;
    }
    foreach($arr as $row){
            $output.= "<table><tr>";
            $output.= "<td>".substr($row['week'],4)."</td>";
            $output.= "<td>".create_bar($row['count'], $max, $row['count'], 170 )."</td>";
            $output.= "</tr></table>";
    }
    return $output;   
}

/**
* generated monthly stats output.
* arguments: @amount = the number of days to show
*            @fid = the file id to base the stats on
*/
function dlc_file_stats_month($amount = 10,$fid) {
    $result = db_query("select * from {dlc_file_month} WHERE fid = %d ORDER BY month DESC LIMIT 0, %d ", array($fid, 10));
    $arr=array();
    $max=0;
    while ($row = db_fetch_array($result)) {
                    if ($max<$row['count'])
                                    $max=$row['count'];
                    $arr[]=$row;
    }
    foreach($arr as $row){
            $output.= "<table><tr>";
            $output.= "<td>".substr($row['month'],0,4).'-'.substr($row['month'],4,2)."</td>";
            $output.= "<td>".create_bar($row['count'], $max, $row['count'], 140 )."</td>";
            $output.= "</tr></table>";
    }
    return $output;      
}

/**
* generated yearly stats output.
* arguments: @amount = the number of days to show
*            @fid = the file id to base the stats on
*/
function dlc_file_stats_year($amount = 10,$fid) {
    $result = db_query("select * from {dlc_file_year} WHERE fid = %d ORDER BY year DESC LIMIT 0, %d ", array($fid, 10));
    $arr=array();
    $max=0;
    while ($row = db_fetch_array($result)) {
                    if ($max<$row['count'])
                                    $max=$row['count'];
                    $arr[]=$row;
    }
    foreach($arr as $row){
            $output.= "<table><tr>";
            $output.= "<td>".$row['year']."</td>";
            $output.= "<td>".create_bar($row['count'], $max, $row['count'], 158 )."</td>";
            $output.= "</tr></table>";
    }
    return $output;   
}

