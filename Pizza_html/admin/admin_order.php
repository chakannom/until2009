<?
	session_start();
	if(!$admin_id && !$admin_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
?>
<html>
<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>관리자 원재료 주문</title>
</head>
<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>
<center>
<table width='100%' height='100%'>
	<tr>
		<td align='center' valign='middle'>
			<br><br>
			<iframe name='menu_list' src='ingredient_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
		<td width='300' height='300' align='center' valign='middle'>
<form name ='admin_order' method='POST' action='order_input.php'>
<font face='HY나무M' size='5'><b>원재료 주문</b></font><br><br>
<table cellpadding='0' cellspacing='0' width='300px' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			기업ID
		</td>
		<td bgcolor='#fcfcfc' align='left'>
			 &nbsp;&nbsp;<font size='3' face='HY나무M'><input type='text' name='c_id' value='' maxlength='10' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			원재료ID
		</td>
		<td bgcolor='#fcfcfc' align='left'>
			&nbsp;&nbsp;<font size='3' face='HY나무M'><input type='text' name='i_id' value='' maxlength='10' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			수량
		</td>
		<td bgcolor='#fcfcfc' align='left'>
			&nbsp;&nbsp;<font size='3' face='HY나무M'><input type='text' name='ds_volume' value='' maxlength='3' style='width:122px'></font>
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
		<td align='center' valign='middle'>
			<br><br>
			<iframe name='company_list' src='company_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
	</tr>
	<tr>
		<td colspan='3' align='center' valign='middle'>
			<br><br>
			<iframe name='menu_list' src='order_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
	</tr>
</table>
</center>
</body>
</html>
