<?
	session_start();

	if(!$com_id && !$com_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
?>

<html>
<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>탈  퇴</title>
<script>
function test(f) {
	if(f.c_id.value.length==0) {
		alert('아이디를 입력해주세요.');
		f.c_id.focus();
		return (false);
	}
	if(f.c_password.value.length==0) {
		alert('패스워드를 입력해주세요.');
		f.c_password.focus();
		return (false);
	}
	return true;		
}

</script>
</head>
<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>

<form name='company_secession' action='company_del.php' method='post' onSubmit='return test(this)'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td align='center' valign='middle'>
<table border='1' width='300' cellspacing='0' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='300' align='center' colspan='2'>
			<font size='6' face='HY나무M'><b>탈  퇴</b></font>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			아이디
		</td>
		<td width='150' align='left'>
			<input type='text' name='c_id' maxlength='10' size='12'>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			패스워드
		</td>
		<td width='150' align='left'>
			<input type='password' name='c_password' maxlength='10' size='12'>
		</td>
	</tr>
	<tr>
		<td width='300' align='center' colspan='2'>
				<input type='submit' name='input' value='  탈 퇴 '> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type='reset' name='reset' value='  취 소  '>
			
		</td>
	</tr>
</table>
		</td>
	</tr>
</table>
</form>

</body>
</html>
