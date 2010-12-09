<?php

function create_bar($width, $show_text = '', $maxnumber, $table_width=550){
	$bar = '';
	if ( strstr($width, '%') ){
		$width = intval(str_replace('%', '', $width));
		if($width > -1){
			$width = ( intval($width) <= 100 ) ? $width . '%' : '100%';
		}
	}
	$width = intval($width * (548 / $maxnumber));
	//make color from width 
	$color = sprintf('#%02X%02X%02X', 20+round($width / 5.8),40 + round($width / 2.8),round($width / 15.8));
	$bar = "<table width=\"".$table_width."\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" height=\"12\">\n";
	$bar .= "\t<tr>\n\t\t<td width=\"".$table_width."\" align=\"left\" style=\"border: 1px solid #333333; background-color: #222222;\" nowrap=\"nowrap\">";
	$bar .= "\n\t\t\t<div align=\"right\" style=\"background-color: {$color}; margin: 0 0 0 0 ; height: 13px; width: {$width}px; font-size:10px\">";
	if ($show_text != "" && $show_text > 0){
		$bar .= "$show_text";
	}
	$bar .= "\n\t\t\t</div>\n\t\t</td>\n\t</tr>\n</table>\n";
	return $bar;
}

/**
	private function, displays an 2 week stat
*/
function _dlc_display_daily($date, $days){
	//normalize date
	$date = $date - ($date % 86400);
	$end = $date + $days * 86400;
	 
	$result = db_query("select * from {dlc_day} where day>=%d and day<=%d", $date, $end );
	$arr=array();
	$max=0;
	while ($row = db_fetch_array($result)) {
			if ($max<$row[count])
					$max=$row[count];
			$arr[]=$row;
	}
	foreach($arr as $row){
		$output.= "<table><tr>";
		$output.= "<td>".strftime("%Y.%m.%d",$row['day'])."</td>";
		$output.= "<td>".create_bar($row[count],$row[count], $max )."</td>";
		$output.= "</tr></table>";
	}
	return $output;
}

/**
	display monthly statistics
*/
function _dlc_display_monthly($date, $months){
	
}

/**
	display daily stats
*/
function dlc_display_daily(){
	$date=1291420800;
	$days=14;
	$monts=24;
	$output.= _dlc_display_daily($date, $days);
	$output.= _dlc_display_monthly($date,24);
	return $output;
}
