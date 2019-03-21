<? 
	session_start();

	session_unregister("login_id");
	session_unregister("login_pass");

	echo("<meta http-equiv='refresh' content='0; url=login.php'>");
?>

