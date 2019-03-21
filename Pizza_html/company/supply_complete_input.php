<?
	session_start();

	include 'oracle_connect.php';

	if(!$ds_orderdate) {
		echo("<SCRIPT>
		window.alert('주문날짜를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$i_name) {
		echo("<SCRIPT>
		window.alert('원재료명을 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$i_unit) {
		echo("<SCRIPT>
		window.alert('단위를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$ds_supplydate) {
		echo("<SCRIPT>
		window.alert('공급날짜를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$ds_price) {
		echo("<SCRIPT>
		window.alert('가격을 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$ds_delivery_person) {
		echo("<SCRIPT>
		window.alert('배달자를 입력하세요');
		history.go(-1);
		</SCRIPT>");
		exit;
	}
	if(!$com_id && !$com_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	$query="BEGIN supply_complete_update('$com_id','$ds_orderdate','$i_name','$i_unit','$ds_supplydate',$ds_price,'$ds_delivery_person',:result); END;";
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

	echo("<meta http-equiv='refresh' content='0; url=supply_complete.php'>");
?>
