<? /* 메뉴 삭제 */ ?>

<html>

<head>
<title>메뉴 삭제</title>
</head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<body>
<center>
<form name ='menu_delete' method='POST' action='menu_del.php'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td height='250' align='center' valign='middle'>
<font face='HY나무M' size='4'><a href='menu_insert.php'>등록</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href='menu_update.php'>수정</a>&nbsp;&nbsp;|&nbsp;&nbsp;삭제</font><br><br>
<font face='HY나무M' size='5'><b>메뉴 삭제</b></font>
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
		<td width='300' align='center' colspan='2'>
			<font size='4' face='HY나무M'><input type='submit' value='   삭 제   '></font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font size='4' face='HY나무M'><input type='reset' value='   취 소   '></font>
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

