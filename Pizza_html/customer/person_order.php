<?
	session_start();
	if(!$loign_id && !$login_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
?>
<html>
<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>고객 피자 주문</title>
</head>
<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red' leftmargin='0' marginwidth='0' topmargin='0' marginheight='0'>

<table width='980' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td width='630' align='center' valign='middle'>
			<iframe name='menu_list' src='../admin/menu_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
		<td width='350' height='170' align='center' valign='middle'>
<form name ='person_order' method='POST' action='order_input.php'>
<font face='HY나무M' size='5'><b>피자 주문</b></font>
<table cellpadding='0' cellspacing='0' width='300px' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			메뉴ID
		</td>
		<td bgcolor='#fcfcfc' align='left'>
			 &nbsp;&nbsp;<font size='3' face='HY나무M'><input type='text' name='m_id' value='' maxlength='10' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			메뉴명
		</td>
		<td bgcolor='#fcfcfc' align='left'>
			&nbsp;&nbsp;<font size='3' face='HY나무M'><input type='text' name='m_name' value='' maxlength='56' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			갯수
		</td>
		<td bgcolor='#fcfcfc' align='left'>
			&nbsp;&nbsp;<font size='3' face='HY나무M'><input type='text' name='po_ordervolume' value='' maxlength='3' style='width:90px'>&nbsp;개</font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			지출방식
		</td>
		<td bgcolor='#fcfcfc' align='left'>
			&nbsp;&nbsp;
			<select name='po_paymethod'>
				<option value=''>지출방식</option>
				<option value='현금'>현금</option>
				<option value='신용카드'>신용카드</option>
			</select>
		</td>
	</tr>
	<tr>
		<td width='300' height='2' align='center' colspan='2'>
			<font size='4' face='HY나무M'><input type='submit' value='   주 문   '></font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font size='4' face='HY나무M'><input type='reset' value='   취 소   '></font>
		</td>
	</tr>

</table>
</form>
		</td>
	</tr>
	<tr>
		<td colspan='2' align='center' valign='middle'>
			<iframe name='menu_list' src='order_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
	</tr>
</table>

</body>
</html>
