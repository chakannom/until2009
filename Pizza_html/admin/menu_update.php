<? /* 메뉴 수정 */ ?>

<html>

<head>
<title>메뉴 수정</title>
</head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<body>
<center>
<form name ='menu_update' method='POST' action='menu_input.php'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td height='250' align='center' valign='middle'>
<font face='HY나무M' size='4'><a href='menu_insert.php'>등록</a>&nbsp;&nbsp;|&nbsp;&nbsp;수정&nbsp;&nbsp;|&nbsp;&nbsp;<a href='menu_delete.php'>삭제</a></font><br><br>
<font face='HY나무M' size='5'><b>메뉴 수정</b></font>
<table cellpadding='0' cellspacing='0' width='300px' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			메뉴ID
		</td>
		<td bgcolor='#fcfcfc' align='center'>
			 <font size='3' face='HY나무M'><input type='text' name='m_id' value='' maxlength='10' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			메뉴명
		</td>
		<td bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='m_name' value='' maxlength='20' style='width:122px'></font>
		</td>
	</tr>
	<tr>
		<td width='100' bgcolor='eef0f4' height='30' align='center'>
			가격
		</td>
		<td bgcolor='#fcfcfc' align='center'>
			<font size='3' face='HY나무M'><input type='text' name='m_price' value='' maxlength='10' style='width:122px'></font>
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
		<td width='300' align='center' colspan='2'>
			<font size='4' face='HY나무M'><input type='submit' value='   수 정   '></font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font size='4' face='HY나무M'><input type='reset' value='   취 소   '></font>
		</td>
	</tr>
</table>
		</td>
	</tr>
	<tr>
		<td align='ceneter' valign='top'>
			<iframe name='menu_list' src='menu_list.php?record_start=0' width='100%' height='100%' vspace='0' hspace='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>
		</td>
	</tr>
</table>
</form>
</center>

</body>
</html>

