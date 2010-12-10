<?php
/* This file is part of Springfiles (GPL v2 or later), see LICENSE.txt */

/**
 * @author abma <spam@abma.de>
 * @file
 * used for the cron job
 */

/**
 *  reads the table download_count and fills dlc_day + dlc_file with the data to generate some statistics
 */

function dlc_cron_run(){
	$lastrun=intval(variable_get("dlc_last_run",0));
	$result=db_query("SELECT dcid,fid,timestamp FROM {download_count} WHERE dcid > %d", $lastrun);
	$updates=array();
	
	//for each entry in the log, increment the stat-tables
	while ($row = db_fetch_array($result)) {
		if ($lastrun<$row['dcid'])
			$lastrun=$row['dcid'];
		$day=$row['timestamp'] - ($row['timestamp'] % 86400); //round to full day

		//insert/update into daily table
		$res=db_query("UPDATE {dlc_day} SET count=count+1 WHERE day=%d",$day);
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

