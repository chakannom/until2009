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
	if(!$m_name) {
		echo("<SCRIPT>
		window.alert('메뉴명을 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$m_size) {
		echo("<SCRIPT>
		window.alert('크기를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$po_ordervolume) {
		echo("<SCRIPT>
		window.alert('주문량을 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$login_id && !$login_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	else {
		$query="BEGIN po_delete('$login_id','$po_orderdate','$m_name','$m_size',$po_ordervolume,:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt);

		if($result=="FALSE") {
			echo("<SCRIPT>
			window.alert('취소할 수 없습니다.');
			history.go(-1);
			</SCRIPT>");
			exit;
		}
		else if($result=="TRUE") {
			echo("<SCRIPT>
			window.alert('취소 되었습니다.');
			</SCRIPT>");
			echo("<meta http-equiv='refresh' content='0; url=order_cancel.php'>");
			exit;
		}
	}

	OCILogOff($conn);
?>
