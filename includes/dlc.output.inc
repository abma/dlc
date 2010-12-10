<?php
/* This file is part of Springfiles (GPL v2 or later), see LICENSE.txt */

/**
 * @author abma <spam@abma.de>
 * @file
 * used for all output of the dlc-module
 */


/**
 *	creates a progressbar
 *	@param $width is % it is filled
 *	@param $maxnumber is the max possible amout
 *	@param $show_text is the text displayed
 *	@param $table_width is the width of the table
 */
function create_bar($width, $maxnumber, $show_text="",$table_width=800){
	$bar = "";
	
	if ($maxnumber)
		$percent = ($width / $maxnumber);
	else
		$percent=0;
	//width in pixel (0-100)
	$width = round($table_width * $percent);
	//brightest color:#114 236  35
	//darkest color:  # 20  40  00
	$color = sprintf('#%02X%02X%02X', 20+round($percent *94),40 + round($percent*196),round($percent*35));

	$bar .= '
	<table style="width: '.$table_width.'px; height:8px; border: 1px solid #333333; " cellpadding="0" cellspacing="0">
	<tr>
		<td style="background-color:#222222; height: 8px;">
			<div style="width:'.$width.'px; background-color:'.$color.'; font-size: 8px; text-align: right; height: 10px; margin: 0 0 0 0; padding: 0 0 1px 0; ">'.$show_text.'</div>
		</td>
	</tr>
	</table>
	';
	
	return $bar;
}

function _dlc_create_head($title){
	return "<h2>".$title."</h2>";
}

/**
 *	displays an 2 week stat
 *	@param $date the last date
 *	@param $days how many days are shown
 */
function _dlc_display_daily($date, $days=14){
	//normalize date
	$output=_dlc_create_head("Downloads last $days days");
	$date = $date - ($date % 86400);
	$end = $date - $days * 86400;
	$result = db_query("select * from {dlc_day} where day<=%d and day>=%d ORDER BY day DESC", $date, $end );
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
		$output.= "<td>".create_bar($row['count'], $max, $row['count'] )."</td>";
		$output.= "</tr></table>";
	}
	return $output;
}

/**
 *	display monthly statistics
 *	@param $date is the startdate
 *	@param $months is count of months to display
 */
function _dlc_display_monthly($date, $months=24){
	$output=_dlc_create_head("Downloads last $months months");
	$montsec=2678400; //seconds a month has
	$enddate=$date - ($montsec*$months);
	$result=db_query("SELECT day, FROM_UNIXTIME(day,'%Y/%m') as month, sum(count) as sumcount FROM {dlc_day} WHERE day<=%d and day>=$enddate GROUP by month ORDER by month DESC",$date, $enddate);
	$arr=array();
	$max=0;
	while ($row = db_fetch_array($result)) {
			if ($max<$row['sumcount'])
					$max=$row['sumcount'];
			$arr[]=$row;
	}
	foreach($arr as $row){
		$output.= "<table><tr>";
		$output.= "<td>".$row['month']."</td>";
		$output.= "<td>".create_bar($row['sumcount'], $max, $row['sumcount'] )."</td>";
		$output.= "</tr></table>";
	}
	return $output;
}

/**
 *	display monthly statistics
 *	@param $date is the startdate
 *	@param $weeks is count of months to display
 */
function _dlc_display_weekly($date, $weeks=52){
	$output=_dlc_create_head("Downloads last $weeks weekls");
	$weeksec=604800; //seconds a week has
	$enddate=$date - ($weeksec*$weeks);
	$result=db_query("SELECT day, FROM_UNIXTIME(day,'%Y/%u') as week, sum(count) as sumcount FROM {dlc_day} WHERE day<=%d and day>=%d GROUP by week ORDER by week DESC",$date, $enddate);
	$arr=array();
	$max=0;
	while ($row = db_fetch_array($result)) {
			if ($max<$row['sumcount'])
					$max=$row['sumcount'];
			$arr[]=$row;
	}
	foreach($arr as $row){
		$output.= "<table><tr>";
		$output.= "<td>".$row['week']."</td>";
		$output.= "<td>".create_bar($row['sumcount'], $max, $row['sumcount'] )."</td>";
		$output.= "</tr></table>";
	}
	return $output;
}

/**
 *	display daily stats
 */
function dlc_display(){
	$date=time();
	$output.= _dlc_display_daily($date);
	$output.= _dlc_display_weekly($date);
	$output.= _dlc_display_monthly($date);
	return $output;
}

