<html>

<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>기업 가입 정보 입력</title>
<script>
function test(f) {
	if(f.c_id.value.length==0) {
		alert('아이디를 입력해주세요.');
		f.c_id.focus();
		return (false);
	}
	if(f.c_password.value.length==0||f.c_repassword.value.length==0) {
		alert('패스워드를 입력해주세요.');
		f.c_password.focus();
		return (false);
	}
	
	if(f.c_name.value.length==0) {
		alert('기업명을 입력해주세요.');
		f.c_name.focus();
		return (false);
	}
	if(f.c_tel.value.length==0) {
		alert('전화번호를 입력해주세요.');
		f.c_tel.focus();
		return (false);
	}
	if(f.c_addr.value.length == 0) {
		alert('주소를 입력해주세요.');
		f.c_addr.focus();
		return (false);
	}
	return true;		
}

function pass() {
	if(document.company_join.c_password.value != document.company_join.c_repassword.value) {
		alert('비밀번호가 일치하지 않습니다.');
		document.company_join.c_password.value='';
		document.company_join.c_repassword.value='';
		document.company_join.c_password.focus();
		return(false);
	}
}

function before() {
	history.go(-1);
}

function idcheck() {
	window.open('id_check.php?c_id=' + document.company_join.c_id.value,'idwin','width=350,height=100,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=no,resizable=yes');
}

</script>
</head>
<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>

<form name='company_join' action='company_input.php' method='post' onSubmit='return test(this)'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
    <tr>
        <td align='center' valign='middle'>
<table border='1' align='center' width='543' cellspacing='0' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='533' align='center' colspan='2'>
			<font size='6'><b>기업 가입 정보 입력</b></font>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			아이디
		</td>
		<td width='377' align='left'>
			<input type='text' name='c_id' maxlength='10' size='12'>&nbsp;<a href='#' onClick='idcheck()'>중복 확인</a>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			패스워드
		</td>
		<td width='377' align='left'>
			<input type='password' name='c_password' maxlength='10' size='12'>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			패스워드 확인
		</td>
		<td width='377' align='left'>
			<input type='password' name='c_repassword' maxlength='10' size='12'>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			이&nbsp;&nbsp;름
		</td>
		<td width='377' align='left'>
			<input type='text' name='c_name' maxlength='20' size='12'>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			연락처
		</td>
		<td width='377"' align='left'>
			<input type='text' name='c_tel' maxlength='16' size='17'> (예: 000-000-0000)
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='EEF0F4'>
			주&nbsp;&nbsp;소
		</td>
		<td width='377' align='left'>
			<input type='text' name='c_addr' maxlength='128' size='50'>
		</td>
	</tr>
	<tr>
		<td width='533' align='center' colspan='2'>
				<input type='submit' name='input' value='   확 인   ' onClick='pass()'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type='reset' name='reset' value='다시 입력'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type='button' name='BT_before' value='전단계로' onClick='before()' >
		</td>
	</tr>
</table>
		</td>
	</tr>
</table>
</form>

</body>
</html>
