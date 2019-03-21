<?
	session_start();
	include 'admin.php';
?>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<?
	if(!$ad_id) {
		echo("<SCRIPT>
			window.alert('아이디를 입력해 주세요.');
			history.go(-1);
		      </SCRIPT>");
		exit;
	}
	if(!$ad_password) {
		echo("<SCRIPT>
			window.alert('패스워드를 입력해 주세요.');
			history.go(-1);
		      </SCRIPT>");
		exit;
	}

	if($ad_id==$admin_id && $ad_password==$admin_pass) {
		session_register("admin_id");
		session_register("admin_pass");

		echo("<meta http-equiv='refresh' content='0; url=admin_index.html'>");
	}
	else {
		echo("<SCRIPT>
			window.alert('아이디와 패스워드가 잘못 되었습니다.');
			history.go(-1);
		      </SCRIPT>");
		exit;
	}
?>
