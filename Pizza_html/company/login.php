<?
	session_start();
?>

<html>

<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>login</title>
</head>
<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red' leftmargin='0' marginwidth='0' topmargin='0' marginheight='0'>

<form name='login' method='post' action='login_check.php'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td align='center' valign='middle'>
<?
	if(!$com_id && !$com_pass) {
?>
<table width='196' height='96' cellpadding='0' cellspacing='0' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='194' height='24' align='center' colspan='2'>
			<font size='4'><b>공급자 모드 로그인</b></font>
		</td>
	</tr>
	<tr>
		<td width='79' height='24' align='right' bgcolor='#eef0f4'>
			아이디
		</td>
		<td width='115' height='24' align='center'>
			<input type='text' name='c_id' maxlength='10' size='12'>
		</td>
	</tr>
	<tr>
		<td width='79' height='24' align='right' bgcolor='#eef0f4'>
			패스워드
		</td>
		<td width='115' height='24' align='center'>
			<input type='password' name='c_password' maxlength='10' size='12'>
		</td>
	</tr>
	<tr>
		<td width='194' height='24' align='center' colspan='2'>
			<a href='company_join.php'>가입</a>&nbsp;&nbsp;&nbsp;<input type='submit' name='login_ok' value=' 로그인 '>&nbsp;&nbsp;&nbsp;<a href='#' onClick='self.close()'>닫기</a>
		</td>
	</tr>
</table>

<?
	}
	else {
?>

<table width='196' height='77' cellpadding='0' cellspacing='0' height='0' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='194' height='24' align='center' colspan='2'>
			<font size='4'><b>공급자 모드 로그인</b></font>
		</td>
	</tr>
	<tr>
		<td width='194' height='25' align='center' bgcolor='#eef0f4'>
			<? echo $com_id; ?>공급자 님
		</td>
	</tr>
	<tr>
		<td width='194' height='25' align='center' bgcolor='#eef0f4'>
			로그인 중입니다
		</td>
	</tr>
	<tr>
		<td width='194' height='25' align='center'>
			<a href='logout.php'>로그아웃</a>
		</td>
	</tr>
</table>

<?
	}
?>
		</td>
	</tr>
</table>
</form>

</body>
</html>

