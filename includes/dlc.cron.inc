<?php

function dlc_cron_run(){
	$lastrun=intval(variable_get("dlc_last_run"));
	$result=db_query("SELECT fid,timestamp FROM {download_count} WHERE dcid > %s", array($lastrun));
	$updates=array();
	
	//for each entry in the log, increment the stat-tables
	while ($row = db_fetch_array($result)) {
		$day=$row['timestamp'] - ($row['timestamp'] % 86400); //round to full day

		//insert/update into daily table
		$res=db_query("UPDATE {dlc_day} SET count=count+1 WHERE day=%d",$row['fid']);
		if (db_affected_rows()<=0)
			$res=db_query("INSERT  INTO {dlc_day} (count, day) VALUES (%d, %d)",1,$day);

		//insert/update into file table
		$res=db_query("UPDATE {dlc_file} SET count=count+1 WHERE fid=%s",$row['fid']);
		if (db_affected_rows()<=0)
			$res=db_query("INSERT  INTO {dlc_file} (count, fid) VALUES (%d, %d)",1,$row['fid']);

	}

	//set start point for next run
	variable_set("dlc_last_run",$lastrun);
}
