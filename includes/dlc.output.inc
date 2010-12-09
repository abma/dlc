<?php

function create_bar($width, $maxnumber, $show_text="",$table_width=300){
	$bar = "";
	$width = round($table_width * ($width / $maxnumber));
	//make color from width 
	$color = sprintf('#%02X%02X%02X', 20+round($width / 5.8),40 + round($width / 2.8),round($width / 15.8));
	
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

/**
	private function, displays an 2 week stat
*/
function _dlc_display_daily($date, $days){
	//normalize date
	$date = $date - ($date % 86400);
	$end = $date + $days * 86400;
	 
	$result = db_query("select * from {dlc_day} where day>=%d and day<=%d ORDER BY day", $date, $end );
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
		$output.= "<td>".create_bar($row[count], $max, $row[count] )."</td>";
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
function dlc_display(){
	$date=1291420800;
	$days=14;
	$monts=24;
	$output.= _dlc_display_daily($date, $days);
	$output.= _dlc_display_monthly($date,24);
	return $output;
}
