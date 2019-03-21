<? /* 피자 주문 목록 */ ?>

<html>

<head>
<title>피자 주문 목록</title>
</head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<body>
<center>
<form name='pizza_order_ok' method='POST' action='pizza_order_ok_input.php'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td height='250' align='center' valign='middle'>
<font face='HY나무M' size='5'><b>배송 체크</b></font>
<table cellpadding='0' cellspacing='0' width='300' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			날짜
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='po_orderdate' value='' maxlength='10' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			메뉴ID
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='m_id' value='' maxlength='10' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			고객ID
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='p_id' value='' maxlength='10' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='300' align='center' colspan='2'>
			<font size='4' face='HY나무M'><input type='submit' value='   배  달   '></font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font size='4' face='HY나무M'><input type='reset' value='   취 소   '></font>
		</td>
	</tr>
</table>
		</td>
	</tr>
	<tr>
		<td align='ceneter' valign='top'>
			<iframe name='pizza_order_list' src='pizza_order_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
	</tr>
</table>
</form>
</center>

</body>
</html>

