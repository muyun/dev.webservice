<?php
//����ϥ� session
session_start();


//�ˬd�Τᦳ�S���n�J
function logincheck()
{
	$userName = (isset($_SESSION[ "userName" ]) ? $_SESSION[ "userName" ] : '');
	if ( $userName == "" )
	{
		Header( "Location: login.php" );
	}
}
?>