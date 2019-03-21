<?
	session_start();
	if(!$admin_id && !$admin_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
 
	include 'oracle_connect.php';

	if(!$m_name) {
		echo("'
<SCRIPT>
window.alert('메뉴명을 입력해 주세요.');
history.go(-1);
</SCRIPT>
		");
		exit;
	}
	if(!$m_price) {
		echo("
<SCRIPT>
window.alert('가격를 입력해 주세요.');
history.go(-1);
</SCRIPT>
		");
		exit;
	}
	if(!$m_size) {
		echo("
<SCRIPT>
window.alert('크기를 입력해 주세요.');
history.go(-1);
</SCRIPT>
		");
		exit;
	}
	if(!$m_id) {
		$query="BEGIN menu_insert('$m_name','$m_price','$m_size',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("
<SCRIPT>
window.alert('메뉴등록을 할수 없습니다. 다시 입력!');
history.go(-1);
</SCRIPT>
			");
			exit;
		}
		OCILogOff($conn);
		echo("<meta http-equiv='refresh' content='0; url=menu_insert.php'>");

	} else {
		$query="BEGIN menu_update('$m_id','$m_name','$m_price','$m_size',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("
<SCRIPT>
window.alert('수정 실패!');
history.go(-1);
</SCRIPT>
			");
			exit;
		}
		OCILogOff($conn);
		echo("<meta http-equiv='refresh' content='0; url=menu_update.php'>");
	}
?>
