<?
	session_start();

	include 'oracle_connect.php';

	if(!$c_id) {
		echo("<SCRIPT>
		window.alert('기업ID를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$c_name) {
		echo("<SCRIPT>
		window.alert('기업명을 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$admin_id && !$admin_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	$query="BEGIN company_ok_update('$c_id','$c_name',:result); END;";
	$stmt=OCIParse($conn,$query); 
	OCIBindByName ($stmt,":result",&$result,30); 
	OCIExecute($stmt);

	if($result=="FALSE") {
		echo("<SCRIPT>
		window.alert('이미 확인이 되어있거나 없는 데이터입니다.');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	OCILogOff($conn);

	echo("<meta http-equiv='refresh' content='0; url=company_ok.php'>");
?>
