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
	if(!$admin_id && !$admin_pass) {
?>
<table width='196' height='96' cellpadding='0' cellspacing='0' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='194' height='24' align='center' colspan='2'>
			<p><font size='4'><b>관리자 모드 로그인</b></font></p>
		</td>
	</tr>
	<tr>
		<td width='79' height='24' align='right' bgcolor='#eef0f4'>
			<p>아이디</p>
		</td>
		<td width='115' height='24' align='center'>
			<p><input type='text' name='ad_id' maxlength='10' size='12'></p>
		</td>
	</tr>
	<tr>
		<td width='79' height='24' align='right' bgcolor='#eef0f4'>
			<p>패스워드</p>
		</td>
		<td width='115' height='24' align='center'>
			<p><input type='password' name='ad_password' maxlength='10' size='12'></p>
		</td>
	</tr>
	<tr>
		<td width='194' height='24' align='center' colspan='2'>            
			<p><input type='submit' name='login_ok' value=' 로그인 '>&nbsp;&nbsp;&nbsp;&nbsp;<a href='#' onClick='self.close()'>닫기</a></p>
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
			<p><font size='4'><b>관리자 모드 로그인</b></font></p>
		</td>
	</tr>
	<tr>
		<td width='194' height='25' align='center' bgcolor='#eef0f4'>
			<p>관리자 님</p>
		</td>
	</tr>
	<tr>
		<td width='194' height='25' align='center' bgcolor='#eef0f4'>
			<p>로그인 중입니다</p>
		</td>
	</tr>
	<tr>
		<td width='194' height='25' align='center'>
			<p><a href='logout.php'>로그아웃</a></p>
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

