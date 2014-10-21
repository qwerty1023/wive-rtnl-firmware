<html>
<head>
<title>IPv6 Settings</title>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Cache-Control" content="no-store, no-cache, must-revalidate, post-check=0, pre-check=0">
<meta http-equiv="Pragma" content="no-cache">

<link rel="stylesheet" href="/style/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/style/controls.css" type="text/css">
<link rel="stylesheet" href="/style/windows.css" type="text/css">

<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/js/validation.js"></script>
<script type="text/javascript" src="/js/share.js"></script>
<script type="text/javascript" src="/js/controls.js"></script>

<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("internet");
var ipv66rdb = "<% getIPv66rdBuilt(); %>";
var ip6to4b = "<% getIP6to4Built(); %>";

function display_on()
{
	if (window.ActiveXObject) { // IE
		return "block";
	}
	else if (window.XMLHttpRequest) { // Mozilla, Firefox, Safari,...
		return "table-row";
	}
}

function initTranslation()
{
	_TRV("lApply", "inet apply");
	_TRV("lCancel", "inet cancel");
}

function SwitchOpMode(form)
{
	var conn_type = form.ipv6_opmode.value;
	displayElement('v6StaticTable', conn_type == '1');
	displayElement('v66rdTable', conn_type == '2');
	displayElement('6to4Table', conn_type == '3');
}

function initValue()
{
	var opmode = "<% getCfgZero(1, "IPv6OpMode"); %>";
	var opmode_len = document.ipv6_cfg.ipv6_opmode.options.length;
	var form = document.ipv6_cfg;

	initTranslation();

	if (ipv66rdb == "1") {
		form.ipv6_opmode.options[2] = new Option("Tunneling Connection (6RD)", "2");
		opmode_len++;
	}
	if (ip6to4b == "1") {
		form.ipv6_opmode.options[opmode_len] = new Option("Tunneling Connection (6TO4)", "3");
		opmode_len++;
	}

	if (opmode == "1")
		form.ipv6_opmode.value = 1;
	else if (ipv66rdb == "1" && opmode == "2")
		form.ipv6_opmode.value = 2;
	else if (ip6to4b == "1" && opmode == "3")
		form.ipv6_opmode.value = 3;
	else form.ipv6_opmode.value = 0;


	form.ipv6_lan_ipaddr.value = "<% getCfgGeneral(1, "IPv6IPAddr"); %>";
	form.ipv6_lan_prefix_len.value = "<% getCfgGeneral(1, "IPv6PrefixLen"); %>";

	form.ipv6_wan_ipaddr.value = "<% getCfgGeneral(1, "IPv6WANIPAddr"); %>";
	form.ipv6_wan_prefix_len.value = "<% getCfgGeneral(1, "IPv6WANPrefixLen"); %>";
	form.ipv6_static_gw.value = "<% getCfgGeneral(1, "IPv6GWAddr"); %>";

	form.ipv6_6rd_prefix.value = "<% getCfgGeneral(1, "IPv6IPAddr"); %>";
	form.ipv6_6rd_prefix_len.value = "<% getCfgGeneral(1, "IPv6PrefixLen"); %>";
	form.ipv6_6rd_border_ipaddr.value = "<% getCfgGeneral(1, "IPv6SrvAddr"); %>";

	form.ipv6_6to4_srv_ipaddr.value = "<% getCfgGeneral(1, "ipv6_6to4_srv_ipaddr"); %>";

	SwitchOpMode(form);

}

function atoi(str, num)
{
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}

	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
}

function checkRange(str, num, min, max)
{
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkIpv4Addr(field)
{
	if (field.value == "") {
		alert("Error. IP address is empty.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if ((!checkRange(field.value, 1, 0, 255)) ||
			(!checkRange(field.value, 2, 0, 255)) ||
			(!checkRange(field.value, 3, 0, 255)) ||
			(!checkRange(field.value, 4, 1, 254)))
	{
		alert('IP adress format error.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	return true;
}

function checkIpv6Addr(ip_addr, len)
{
	var ip_item = new Array();
	ip_item = ip_addr.split(":");
	for (var i=0; i<ip_item.length; i++) {
		if (parseInt(ip_item[i], 16) == "NaN") {
			alert('It should be a [0-F] number.');
			return false;
		}
	}

	return true;
}

function CheckValue()
{
	if (document.ipv6_cfg.ipv6_opmode.value == "1") {
		if (document.ipv6_cfg.ipv6_lan_ipaddr.value == "" &&
		    document.ipv6_cfg.ipv6_wan_ipaddr.value == "") {
			alert("please fill LAN/WAN IPv6 Address!");
			return false;
		}
		if (document.ipv6_cfg.ipv6_lan_ipaddr.value != "") {
			if (!checkIpv6Addr(document.ipv6_cfg.ipv6_lan_ipaddr.value, 128)) {
				alert("invalid IPv6 IP address!");
				document.ipv6_cfg.ipv6_lan_ipaddr.focus();
				document.ipv6_cfg.ipv6_lan_ipaddr.select();
				return false;
			}
			if (document.ipv6_cfg.ipv6_lan_prefix_len.value == "" || 
			    document.ipv6_cfg.ipv6_lan_prefix_len.value > 128 || 
			    document.ipv6_cfg.ipv6_lan_prefix_len.value < 0) {
				alert("invalid prefix length!");
				document.ipv6_cfg.ipv6_lan_prefix_len.focus();
				document.ipv6_cfg.ipv6_lan_prefix_len.select();
				return false;
			}
		}
		if (document.ipv6_cfg.ipv6_wan_ipaddr.value != "") {
			if (!checkIpv6Addr(document.ipv6_cfg.ipv6_wan_ipaddr.value, 128)) {
				alert("invalid IPv6 IP address!");
				document.ipv6_cfg.ipv6_wan_ipaddr.focus();
				document.ipv6_cfg.ipv6_wan_ipaddr.select();
				return false;
			}
			if (document.ipv6_cfg.ipv6_wan_prefix_len.value == "" ||
			    document.ipv6_cfg.ipv6_wan_prefix_len.value > 128 || 
			    document.ipv6_cfg.ipv6_wan_prefix_len.value < 0) {
				alert("invalid prefix length!");
				document.ipv6_cfg.ipv6_wan_prefix_len.focus();
				document.ipv6_cfg.ipv6_wan_prefix_len.select();
				return false;
			}
		}
		if (document.ipv6_cfg.ipv6_static_gw.value != "" &&
		    (!checkIpv6Addr(document.ipv6_cfg.ipv6_static_gw.value, 128))) {
			alert("invalid IPv6 IP address!");
			document.ipv6_cfg.ipv6_static_gw.focus();
			document.ipv6_cfg.ipv6_static_gw.select();
			return false;
		}
	} else if (document.ipv6_cfg.ipv6_opmode.value == "2") {
		if (document.ipv6_cfg.ipv6_6rd_prefix.value == "" ||
		    document.ipv6_cfg.ipv6_6rd_prefix_len.value == "" ||
		    document.ipv6_cfg.ipv6_6rd_border_ipaddr.value == "") {
			alert("please fill all fields!");
			return false;
		}
		if (!checkIpv6Addr(document.ipv6_cfg.ipv6_6rd_prefix.value, 32)) {
			alert("invalid IPv6 IP address!");
			document.ipv6_cfg.ipv6_6rd_prefix.focus();
			document.ipv6_cfg.ipv6_6rd_prefix.select();
			return false;
		}
		if (document.ipv6_cfg.ipv6_6rd_prefix_len.value > 32 || 
		    document.ipv6_cfg.ipv6_6rd_prefix_len.value < 0) {
			alert("invalid prefix length!");
			document.ipv6_cfg.ipv6_6rd_prefix_len.focus();
			document.ipv6_cfg.ipv6_6rd_prefix_len.select();
			return false;
		}
		if (!checkIpv4Addr(document.ipv6_cfg.ipv6_6rd_border_ipaddr.value)) {
			alert("invalid IPv4 ip address!");
			document.ipv6_cfg.ipv6_6rd_border_ipaddr.focus();
			document.ipv6_cfg.ipv6_6rd_border_ipaddr.select();
			return false;
		}
	} else if (document.ipv6_cfg.ipv6_opmode.value == "3") {
		if (document.ipv6_cfg.ipv6_6to4_srv_ipaddr.value == "" ) {
			alert("Please fill all fields!");
			return false;
		}
		if (!checkIpv4Addr(document.ipv6_cfg.ipv6_6to4_srv_ipaddr.value)) {
			alert("invalid IPv4 ip address!");
			document.ipv6_cfg.ipv6_6to4_srv_ipaddr.focus();
			document.ipv6_cfg.ipv6_6to4_srv_ipaddr.select();
			return false;
		}
	}
	return true;
}

</script>
</head>

<body onLoad="initValue()">

<table class="body"><tr><td>

<h1 id="IPv6 Network Settings">IPv6 Network Settings</h1>
<p id="Setup your IPv6 network">Setup your IPv6 network</p>
<hr>

<form method="POST" name="ipv6_cfg" action="/goform/setIPv6" onSubmit="return CheckValue(this)">

<table class="form">
<tr>
	<td class="title" colspan="2">IPv6 connection type</td>
</tr>
<tr>
	<td class="head" id="v6OpMode">IPv6 Operation Mode</td>
	<td>
		<select name="ipv6_opmode" class="mid" onChange="SwitchOpMode(this.form)">
			<option value="0" id="v6Disable">Disable</option>
			<option value="1" id="v6Static">Native dynamic/static IP Connection</option>
		</select>
	</td>
</tr>
</table>

<!-- === STATIC IP === -->
<table id="v6StaticTable" class="form">
<tr>
	<td class="title" colspan="2" id="v6StaticIPSetup">IPv6 Static IP Setup</td>
</tr>
<tr>
	<td class="head" id="v6StaticIPAddr">LAN IPv6 Address / Subnet Prefix Length</td>
	<td><input name="ipv6_lan_ipaddr" class="mid" maxlength=39 size=27> / <input name="ipv6_lan_prefix_len" maxlength=3 size=2></td>
</tr>
<tr>
	<td class="head" id="v6StaticIPAddr">WAN IPv6 Address / Subnet Prefix Length</td>
	<td><input name="ipv6_wan_ipaddr" class="mid" maxlength=39 size=27> / <input name="ipv6_wan_prefix_len" maxlength=3 size=2></td>
</tr>
<tr>
	<td class="head" id="v6StaticGW">Default Gateway</td>
	<td><input name="ipv6_static_gw" class="mid" maxlength=39 size=27></td>
</tr>
</table>

<!-- 6RD -->
<table id="v66rdTable" class="form">
<tr>
  <td class="title" colspan="2" id="v66rdSetup">Tunneling Connection (6RD) Setup</td>
</tr>
<tr>
  <td class="head" id="v66rdPrefix">ISP 6rd Prefix / Prefix Length</td>
  <td><input name="ipv6_6rd_prefix" maxlength=9 size=7> / <input name="ipv6_6rd_prefix_len" maxlength=3 size=2></td>
</tr>
<tr>
  <td class="head" id="v66rdBorderIPAddr">ISP Border Relay IPv4 Address</td>
  <td><input name="ipv6_6rd_border_ipaddr" maxlength=15 size=13></td>
</tr>
</table>
<!-- 6to4 -->
<table id="6to4Table" class="form">
<tr>
  <td class="title" colspan="2" id="6to4Setup">Tunneling Connection (6to4) Setup</td>
</tr>
<tr>
  <td class="head" id="v66to4SrvIpaddr"> IPv4 to IPv6 server address </td>
  <td><input name="ipv6_6to4_srv_ipaddr" maxlength=39 size=27></td>
</tr>
</table>

<table class="buttons">
<tr>
	<td>
		<input type="submit" class="normal" value="Apply" id="lApply">&nbsp;&nbsp;
		<input type="reset" class="normal" value="Cancel" id="lCancel" onClick="window.location.reload();">
	</td>
</tr>
</table>
</form>

<div class="whitespace">&nbsp;</div>

</td></tr></table>
</body>
</html>

