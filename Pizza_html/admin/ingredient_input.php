<?
	session_start();
	if(!$admin_id && !$admin_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
 
	include 'oracle_connect.php';

	if(!$i_name) {
		echo("'
<SCRIPT>
window.alert('원재료명을 입력해 주세요.');
history.go(-1);
</SCRIPT>
		");
		exit;
	}
	if(!$i_unit) {
		echo("
<SCRIPT>
window.alert('단위를 입력해 주세요.');
history.go(-1);
</SCRIPT>
		");
		exit;
	}
	if(!$i_id) {
		$query="BEGIN ingredient_insert('$i_name','$i_unit',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("
<SCRIPT>
window.alert('원재료등록을 할수 없습니다. 다시 입력!');
history.go(-1);
</SCRIPT>
			");
			exit;
		}
		OCILogOff($conn);
		echo("<meta http-equiv='refresh' content='0; url=ingredient_insert.php'>");

	} else {
		$query="BEGIN menu_update('$i_id','$i_name','$i_unit',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("
<SCRIPT>
window.alert('원재료수정 실패!');
history.go(-1);
</SCRIPT>
			");
			exit;
		}
		OCILogOff($conn);
		echo("<meta http-equiv='refresh' content='0; url=ingredient_update.php'>");
	}
?>
