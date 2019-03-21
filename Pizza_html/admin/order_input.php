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
	if(!$i_id) {
		echo("<SCRIPT>
		window.alert('원재료ID를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$ds_volume) {
		echo("<SCRIPT>
		window.alert('수량을 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}

	$query="BEGIN ds_insert('$c_id','$i_id',$ds_volume,:result); END;";
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
	echo("<meta http-equiv='refresh' content='0; url=admin_order.php'>");
?>
