<?
	session_start();

	include 'oracle_connect.php';

	if(!$com_id && !$com_pass) {
		$query="BEGIN company_insert('$c_id','$c_password','$c_name','$c_tel','$c_addr',:result); END;";
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
		$query="BEGIN company_update('$com_id','$com_pass','$c_name','$c_tel','$c_addr',:result); END;";
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

	echo("<meta http-equiv='refresh' content='0; url=company_profile.php'>");
?>
