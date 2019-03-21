<? 	/* 로그인 처리 부분 */
	session_start();
	include 'oracle_connect.php'; //서버연결
?>

<?
	if(!$p_id) {
		echo("<SCRIPT>
			window.alert('아이디를 입력해 주세요.');
			history.go(-1);
		      </SCRIPT>");
		exit;
	}
	if(!$p_password) {
		echo("<SCRIPT>
			window.alert('패스워드를 입력해 주세요.');
			history.go(-1);
		      </SCRIPT>");
		exit;
	}


	// 오라클에 프로시저 쿼리 날리는 부분
	
	$query="BEGIN p_id_passwd_find('$p_id','$p_password',:result); END;";
	$stmt=OCIParse($conn,$query); 
	OCIBindByName ($stmt,":result",&$result,30); 
	OCIExecute($stmt);
	OCILogOff($conn);

	if($result=="TRUE") {
		$login_id=$p_id;
		$login_pass=$p_password;
		session_register("login_id");
		session_register("login_pass");

		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	else {
		echo("<SCRIPT>
			window.alert('아이디와 패스워드가 잘못 되었습니다.');
			history.go(-1);
		      </SCRIPT>");
		exit;
	}

?>

