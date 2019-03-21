<? 
	session_start();

	session_unregister("com_id");
	session_unregister("com_pass");

	echo("<meta http-equiv='refresh' content='0; url=login.php'>");
?>

