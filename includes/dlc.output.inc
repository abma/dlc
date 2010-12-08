<?php

function create_bar($width, $show_text = '', $maxnumber)
{
    $bar = '';
    if ( strstr($width, '%') )
    {
        $width = intval(str_replace('%', '', $width));
        if($width > -1)
        {
            $width = ( intval($width) <= 100 ) ? $width . '%' : '100%';
        }
    }

    if($width > -1)
    {

        $width = $width * (548 / $maxnumber);
        //make color from width 
        $color = sprintf('#%02X%02X%02X', 20+round($width / 5.8),40 + round($width / 2.8),round($width / 15.8));
        $bar = "<table width=\"550\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" height=\"12\">\n";
        $bar .= "\t<tr>\n\t\t<td width=\"100%\" align=\"left\" style=\"border: 1px solid #333333; background-color: #222222;\" nowrap=\"nowrap\">";
        $bar .= "\n\t\t\t<div align=\"right\" style=\"background-color: {$color}; margin: 0px; height: 10px; width: {$width}px;\">";
        if ($show_text != "" && $show_text > 0)
        {
            $bar .= "\n\t\t\t\t<span align=\"right\" style=\"font-size: 9px; color: Black;\">$show_text</span>";
        }
        $bar .= "\n\t\t\t</div>\n\t\t</td>\n\t</tr>\n</table>";
    }
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
	while ($row = db_fetch_array($result)) {
			var_dump($row);
	}
}

/**
	display daily stats
*/
function dlc_display_daily(){
	$date=1291420800;
	$days=14;
	_dlc_display_daily($date, $days);
}

