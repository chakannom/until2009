<? /* 오라클 서버에 접속 */
	putenv("NLS_LANG=KOREAN_KOREA.AL32UTF8");
	$db="(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521)))(CONNECT_DATA=(SID=XE)))";
	$conn=OCILogOn("pizza","kgupizza",$db);

	echo("<meta http-equiv='content-type' content='text/html; charset=utf-8'>");
?>
