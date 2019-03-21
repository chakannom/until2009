<?
	session_start();

	if(!$login_id && !$login_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
?>

<html>
<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>탈  퇴</title>
<script>
function test(f) {
	if(f.p_id.value.length==0) {
		alert('아이디를 입력해주세요.');
		f.p_id.focus();
		return (false);
	}
	if(f.p_password.value.length==0||f.p_repassword.value.length==0) {
		alert('패스워드를 입력해주세요.');
		f.p_password.focus();
		return (false);
	}
	return true;		
}

</script>
</head>
<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>

<form name='person_secession' action='person_del.php' method='post' onSubmit='return test(this)'>
<table width='980' cellpadding='0' cellspacing='0'>
	<tr>
		<td width='980' align='center'>
<table border='1' width='300' cellspacing='0' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='300' align='center' colspan='2'>
			<p><font size='6'><b>탈  퇴</b></font></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>아이디</p>
		</td>
		<td width='150' align='left'>
			<p><input type='text' name='p_id' maxlength='10' size='12'></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>패스워드</p>
		</td>
		<td width='150' align='left'>
			<p><input type='password' name='p_password' maxlength='10' size='12'></p>
		</td>
	</tr>
	<tr>
		<td width='300' align='center' colspan='2'>
				<p><input type='submit' name='input' value='  탈 퇴 '> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='../main.html'>메인으로</a></p>
			
		</td>
	</tr>
</table>
		</td>
	</tr>
</table>
</form>

</body>
</html>
