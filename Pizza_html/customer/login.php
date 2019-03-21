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
	if(!$login_id && !$login_pass) {
?>
<table width='196' height='72' cellpadding='0' cellspacing='0' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='79' height='24' align='right' bgcolor='#eef0f4'>
			<p>아이디</p>
		</td>
		<td width='115' height='24' align='center'>
			<p><input type='text' name='p_id' maxlength='10' size='12'></p>
		</td>
	</tr>
	<tr>
		<td width='79' height='24' align='right' bgcolor='#eef0f4'>
			<p>패스워드</p>
		</td>
		<td width='115' height='24' align='center'>
			<p><input type='password' name='p_password' maxlength='10' size='12'></p>
		</td>
	</tr>
	<tr>
		<td width='194' height='24' align='center' colspan='2'>            
			<p><input type='submit' name='login_ok' value=' 로그인 '>&nbsp;&nbsp;&nbsp;&nbsp;<a href='person_join.php' target='main'>회원가입</a></p>
		</td>
	</tr>
</table>

<?
	}
	else {
?>

<table width='196' height='77' cellpadding='0' cellspacing='0' height='0' border='1' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='194' height='25' align='center' bgcolor='#eef0f4'>
			<p><? echo $login_id ?> 님</p>
		</td>
	</tr>
	<tr>
		<td width='194' height='25' align='center' bgcolor='#eef0f4'>
			<p>로그인 중입니다</p>
		</td>
	</tr>
	<tr>
		<td width='194' height='25' align='center'>
			<p><a href='person_profile.php' target='main'>개인정보</a>&nbsp;&nbsp;<a href='logout.php'>로그아웃</a></p>
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

