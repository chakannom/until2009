<? /* 주문 취소 */ 

	session_start();

	if(!$login_id && !$login_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}

?>

<html>

<head>
<title>주문 취소</title>
</head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<body>
<center>
<form name ='order_cancel' method='POST' action='order_del.php'>
<table width='980' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td height='100' align='center' valign='middle'>
<font face='HY나무M' size='5'><b>주문 취소</b></font>
<table cellpadding='0' cellspacing='0' width='300px' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			날짜
		</td>
		<td bgcolor='#fcfcfc' align='center'>
			 <font size='3' face='HY나무M'><input type='text' name='po_orderdate' value='' maxlength='20' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			메뉴명
		</td>
		<td bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='m_name' value='' maxlength='56' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			크기
		</td>
		<td bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='m_size' value='' maxlength='4' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			주문량
		</td>
		<td bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='po_ordervolume' value='' maxlength='3' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='300' align='center' colspan='2'>
			<font size='4' face='HY나무M'><input type='submit' value='   삭 제   '></font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font size='4' face='HY나무M'><input type='reset' value='   취 소   '></font>
		</td>
	</tr>
</table>
		</td>
	</tr>
	<tr>
		<td align='center' valign='top'>
			<iframe name='order_list' src='order_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
	</tr>
</table>
</form>
</center>

</body>
</html>

