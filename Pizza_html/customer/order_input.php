<?
	session_start();

	include 'oracle_connect.php';
	if(!$m_id) {
		echo("<SCRIPT>
		window.alert('메뉴ID를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$m_name) {
		echo("<SCRIPT>
		window.alert('메뉴명을 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$po_ordervolume) {
		echo("<SCRIPT>
		window.alert('갯수를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$po_paymethod) {
		echo("<SCRIPT>
		window.alert('지불방식을 선택하세요.');
		history.go(-1);
		</SCRIPT>");
		exit;
	}

	$query="BEGIN po_insert('$login_id','$m_id','$m_name',$po_ordervolume,'$po_paymethod',:result); END;";
	$stmt=OCIParse($conn,$query); 
	OCIBindByName ($stmt,":result",&$result,30); 
	OCIExecute($stmt);

	if($result=="FALSE") {
		echo("<SCRIPT>
		window.alert('주문 실패');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	OCILogOff($conn);
	echo("<meta http-equiv='refresh' content='0; url=person_order.php'>");
?>
