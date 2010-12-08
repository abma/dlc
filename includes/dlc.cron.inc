<?php

function dlc_cron_run(){
	$lastrun=intval(variable_get("dlc_last_run"));
	$lastrun=0; //FIXME: remove that
	variable_set("dlc_last_run",$lastrun); //better we lose some stats than we double-count it
	$result=db_query("SELECT fid,timestamp FROM {download_count} WHERE dcid > $lastrun ");
	while ($row = db_fetch_array($result)) {
			//now fill data with results
			var_dump($row);
	}
	
	
}
