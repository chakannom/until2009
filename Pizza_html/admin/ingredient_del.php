<?
	session_start();

	include 'oracle_connect.php';

	if(!$admin_id && !$admin_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	else {
		$query="BEGIN ingredient_delete('$i_id','$i_name',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("<SCRIPT>
			window.alert('원재료를 실패했습니다.');
			history.go(-1);
			</SCRIPT>");
			exit;
		}
		else if($result=="TRUE") {
			echo("<SCRIPT>
			window.alert('원재료가 삭제 되었습니다.');
			</SCRIPT>");

			echo("<meta http-equiv='refresh' content='0; url=ingredient_delete.php'>");
			exit;
		}
	}

	OCILogOff($conn);
?>
