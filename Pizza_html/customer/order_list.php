<?
	session_start();

	if(!$login_id && !$login_pass) {
		echo("
<meta http-equiv='refresh' content='0; url=login.php'>
		");
	}

	include 'oracle_connect.php';

	//한 페이지에 출력되는 레코드
	$record_count=4;
	//한 페이지에 출력되는 페이지 링크 수
	$link_count=5;
	//현재 그룹의 시작 레코드 번호 -할당되지 않았을 경우 0로 설정
	if(!$record_start) {
		$record_start=0;
	}
	//현재 페이지 번호
	$current_page=($record_start/$record_count)+1;
	//레코드가 $record_count*$link_count개 증가시마다 그룹 번호가 1씩 증가
	$group=floor($record_start/($record_count*$link_count));
	//게시판 조회 질의
	$query="SELECT * FROM pizza_order WHERE p_id='$login_id'";
	$stmt=OCIParse($conn,$query);
	OCIExecute($stmt);
	//전체 레코드 수
	$total_record=OCIFetchStatement($stmt,&$result);
	//전체 페이지 수
	$total_page=(int)($total_record/$record_count);
	//마지막 페이지의 경계를 초과한 데이터가 있는 경우 전체 페이지 1증가
	if($total_record%$record_count) {
		$total_page++;
	}
	$query="SELECT TO_CHAR(po.po_orderdate,'YYYY-MM-DD'),m.m_name,m.m_size,po.po_ordervolume,(m.m_price*po.po_ordervolume),po.po_paymethod,po.po_delivery_state FROM pizza_order po,(SELECT po_orderdate,rownum AS rnum FROM pizza_order WHERE p_id='$login_id') r, menu m WHERE po.po_orderdate=r.po_orderdate AND po.m_id=m.m_id AND r.rnum BETWEEN $record_start+1 AND $record_start+$record_count";
	$stmt=OCIParse($conn,$query);
	OCIExecute($stmt);
	OCIFetchStatement($stmt,&$result);
	OCILogOff($conn);

	echo("
<html>
<hrad>
<title>고객 주문 리스트</title>
</head>
<body>

<center>
<!- 글 개수와 페이지 번호 출력 ->
<table border='0' width='666' style='table-layout:fixed;' cellpadding='0' cellspacing='0'>
	<tr height='20' bgcolor='#ca97b3'>
		<td width='203' align='center'>
			<font size='2' face='HY나무M' color='white'><b>총 갯수:$total_record</b></font>
		</td>
		<td width='463' align='right'>
			<font size='2' face='HY나무M' color='white'><b>$current_page/$total_page</b></font>
		</td>
	</tr>
</table>

<!- 레코드 제목 출력 ->
<table border='0' width='660' style='table-layout:fixed;'>
	<tr height='20' bgcolor='#eef0f4'>
		<td width='100' align='center'>
			<font size='2' face='HY나무M'><b>날짜</b></font>
		</td>
		<td width='200' align='center'>
			<font size='2' face='HY나무M'><b>메뉴명</b></font>
		</td>
		<td width='50' align='center'>
			<font size='2' face='HY나무M'><b>크기</b></font>
		</td>
		<td width='50' align='center'>
			<font size='2' face='HY나무M'><b>주문량</b></font>
		</td>
		<td width='100' align='center'>
			<font size='2' face='HY나무M'><b>지불가격</b></font>
		</td>
		<td width='100' align='center'>
			<font size='2' face='HY나무M'><b>지불방식</b></font>
		</td>
		<td width='60' align='center'>
			<font size='2' face='HY나무M'><b>배달중</b></font>
		</td>
	</tr>
	");


//레코드의 내용을 출력
for($i=0;$i<$record_count;$i++) {
	reset($result);

	echo("
	<tr>
	");
	while($columns=each($result)) {
		echo("
		<td bgcolor='#f5fdfd' align='center'>
			".$columns["value"][$i]."
		</td>
		");
	}
	echo("
	</tr>
	<tr height='1'>
		<td bgcolor='#ffeef7'>
		</td>
	</tr>
	");
}
	echo("
</table>
	");
//레코드의 총 개수가 한 페이지에 표현될 최대 레코드의 수보다 클 경우
if($total_record>$record_count) {
	//전체 페이지가 2페이지 이상이고 현재 페이니가 처음 페이지가 아니면 좌측 화살표 표시
	if(($total_page>1) && ($record_start!=0)) {
		$pre_page_start=$record_start-$record_count;
		echo("
<a href='order_list.php?record_start=$pre_page_start'><font size='3'>◀</font></a>&nbsp;&nbsp;&nbsp;
		");
	}
	//한 그룹에서 표현할 수 있는 레코드의 개수보다 현제 페이지 직전까지 표현될 수 있는 레코드의 개수가 클 경우
	//이전 그룹으로의 링크 표시
	if($current_page*$record_count > $record_count*$link_count) {
		$pre_group_start=($group*$record_count*$link_count)-$record_count;
		echo("
<a href='order_list.php?record_start=$pre_group_start'>[이전 $link_count 개]</a>&nbsp;&nbsp;&nbsp;
		");
	}
	//현재 그룹에서 링크할 페이지 링크를 반복적으로 출력
	for($i=0;$i<$link_count;$i++) {
		//$record_start로 들어갈 값
		$input_start=($group*$link_count+$i)*$record_count;
		//페이지 링크 번호 계산
		$link=($group*$link_count+$i)+1;
		//$record_start에 들어갈 값이 전체 레코드 수보자 작을 때 링크 표시
		//레코드가 $record_count 값보다 작으면 링크를 표시하지 않음
		if($input_start<$total_record) {
			if($input_start!=$record_start) {
				echo("
<a href='order_list.php?record_start=$input_start'><font size='3'>$link</font></a>&nbsp;&nbsp;
				");
			} else {
				echo("
<font size='3' color='silver'>$link</font>&nbsp;&nbsp;
				");
			}
		}
	}
	if($total_record>(($group+1)*$record_count*$link_count)) {
		$next_group_start=($group+1)*$record_count*$link_count;
		echo("
<a href='order_list.php?record_start=$next_group_start'>[다음 $link_count 개]</a>
		");
	}
	//전체 페이지가 2개 이상이고 현재 페이지가 마지막 페이지가 아니면 우측 화살표 표시
	if(($total_page>1) && ($record_start!=($total_page*$record_count-$record_count))) {
		$next_page_start=$record_start+$record_count;
		echo("
&nbsp;&nbsp;&nbsp;<a href='order_list.php?record_start=$next_page_start'><font size='3'>▶</font></a>
		");
	}
}
	echo("
<p>
<table>
	<tr>
		<td align='center'>
			<form action='order_list.php?record_start=$record_start' method='post'>
			<select name='field'>
				<option value='DATE'>날짜</option>
				<option value='NAME'>메뉴명</option>
				<option value='SIZE'>사이즈</option>
				<option value='ORDERVOLUME'>주문량</option>
				<option value='PRICE'>지불가격</option>
				<option value='PAYMETHOD'>지불방식</option>
				<option value='DELIVERY'>배달중</option>
			</select>
			<input type='text' name='search' size='15'>
			<input type='submit' value='검색'>
			</from>
		</td>
	</tr>
</table>
</p>
</center>

</body>
</html>
	");

?>
