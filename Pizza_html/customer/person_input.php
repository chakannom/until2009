<?
	session_start();

	include 'oracle_connect.php';

	if(!$login_id && !$login_pass) {
		$query="BEGIN person_insert('$p_id','$p_password','$p_name','$p_tel','$p_addr',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("<SCRIPT>
			window.alert('등록을 할수 없습니다. 다시 입력!');
			history.go(-1);
			</SCRIPT>");
			exit;
		}
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	else {
		$query="BEGIN person_update('$login_id','$login_pass','$p_name','$p_tel','$p_addr',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("<SCRIPT>
			window.alert('수정 실패!');
			history.go(-1);
			</SCRIPT>");
			exit;
		}
	}

	OCILogOff($conn);

	echo("<meta http-equiv='refresh' content='0; url=person_profile.php'>");
?>
