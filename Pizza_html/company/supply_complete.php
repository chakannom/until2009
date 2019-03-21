<? /* 공급완료체크 */ ?>

<html>

<head>
<title>공급 완료 체크</title>
</head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<body>
<center>
<form name ='supply_complete' method='POST' action='supply_complete_input.php'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td height='250' align='center' valign='middle'>
<font face='HY나무M' size='5'><b>공급 완료 체크</b></font>
<table cellpadding='0' cellspacing='0' width='600' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			주문날짜
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='ds_orderdate' value='' maxlength='20' style='width:122px'></font>
		</td>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			공급날짜
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='ds_supplydate' value='' maxlength='20' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			원재료명
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='i_name' value='' maxlength='10' style='width:122px'></font>
		</td>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			가격
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='ds_price' value='' maxlength='20' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			단위
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='i_unit' value='' maxlength='4' style='width:122px'></font>
		</td>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			배달자
		</td>
		<td width='200' bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='ds_delivery_person' value='' maxlength='20' style='width:122px'></font>
		</td>
		
	</tr>
	<tr>
		<td width='600' align='center' colspan='4'>
			<font size='4' face='HY나무M'><input type='submit' value='   확  인   '></font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font size='4' face='HY나무M'><input type='reset' value='   취 소   '></font>
		</td>
	</tr>
</table>
		</td>
	</tr>
	<tr>
		<td align='ceneter' valign='top'>
			<iframe name='delivery_supply_list' src='delivery_supply_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
	</tr>
</table>
</form>
</center>

</body>
</html>

