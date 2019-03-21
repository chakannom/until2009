<?
	session_start();

	include 'oracle_connect.php';

	if(!$login_id && !$login_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	else {
		$query="BEGIN person_delete('$p_id','$p_password',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("<SCRIPT>
			window.alert('탈퇴가 되지 않았습니다.');
			history.go(-1);
			</SCRIPT>");
			exit;
		}
		else if($result=="TRUE") {
			echo("<SCRIPT>
			window.alert('탈퇴 되었습니다.');
			</SCRIPT>");
			session_unregister("login_id");
			session_unregister("login_pass");

			echo("<meta http-equiv='refresh' content='0; url=../main.html'>");
			exit;
		}
	}

	OCILogOff($conn);
?>
