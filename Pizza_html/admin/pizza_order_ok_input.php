<?
	session_start();

	include 'oracle_connect.php';

	if(!$po_orderdate) {
		echo("<SCRIPT>
		window.alert('날짜를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$m_id) {
		echo("<SCRIPT>
		window.alert('메뉴ID를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$p_id) {
		echo("<SCRIPT>
		window.alert('고객ID를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$admin_id && !$admin_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	$query="BEGIN po_update('$po_orderdate','$m_id','$p_id',:result); END;";
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

	echo("<meta http-equiv='refresh' content='0; url=pizza_order_ok.php'>");
?>
