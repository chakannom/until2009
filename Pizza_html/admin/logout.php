<? 
	session_start();

	session_unregister("admin_id");
	session_unregister("admin_pass");

	echo("<meta http-equiv='refresh' content='0; url=login.php'>");
?>

