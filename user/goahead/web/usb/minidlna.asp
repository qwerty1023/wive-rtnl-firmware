<html>
<head>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Cache-Control" content="no-store, no-cache, must-revalidate, post-check=0, pre-check=0">
<meta http-equiv="Pragma" content="no-cache">

<link rel="stylesheet" href="/style/normal_ws.css" type="text/css">
<link rel="stylesheet" href="/style/controls.css" type="text/css">
<link rel="stylesheet" href="/style/windows.css" type="text/css">

<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/js/validation.js"></script>
<script type="text/javascript" src="/js/controls.js"></script>

<title>Transmission Settings</title>

<script language="Javascript" type="text/javascript">
function initValue(form)
{
	form.DlnaEnabled.value = defaultNumber("<% getCfgZero(1, "DlnaEnabled"); %>", '0');
	DlnaEnabledSwitch(form);
}

function DlnaEnabledSwitch(form)
{
	disableElement( [ form.dlnaPort, form.dlnastart, form.dlnastop, form.dlnarescan,
	form.dlnaDBPath, form.dlna0Path, form.dlna1Path, form.dlna2Path] , form.DlnaEnabled.value != '1');
}

function CheckValue(form)
{
	if ((document.formDlna.DlnaEnabled.value == '1'))
	{
		if (document.formDlna.dlnaPort.value == "")
		{
			alert('Please specify DLNA port');
			document.formDlna.dlnaPort.focus();
			document.formDlna.dlnaPort.select();
			return false;
		}
		else if (isNaN(document.formDlna.dlnaPort.value) ||
			 parseInt(document.formDlna.dlnaPort.value,10) > 65535 ||
			 parseInt(document.formDlna.dlnaPort.value,10) < 1024)
			{
				alert('Please specify valid DLNA port number');
				document.formDlna.dlnaPort.focus();
				document.formDlna.dlnaPort.select();
				return false;
			}
		
		if (document.formDlna.dlnaDBPath.value == "")
		{
			alert('Please specify DLNA DB path');
			document.formDlna.dlnaDBPath.focus();
			document.formDlna.dlnaDBPath.select();
			return false;
		}
	}
	return true;
}

function submit_apply(parm)
{		
		if (parm == "apply")
			{
				if (CheckValue(this.form) != true)
				{
					return;
				}
			}
		document.formDlna.hiddenButton.value = parm;
		document.formDlna.submit();	
}

</script>
</head>

<body onLoad="initValue(document.formDlna)">
<table class="body"><tr><td>

<h1>DLNA settings</h1>
<p>Here you can configure DLNA server.</p>
<hr>

<form action="/goform/formDlna" method="POST" name="formDlna">
<input type=hidden name="hiddenButton" value="">
<table class="form">
<tr>
	<td class="title" colspan="2">miniDLNA Settings</td>
</tr>
<tr>
	<td class="head">Enable DLNA</td>
	<td>
		<select name="DlnaEnabled" class="half" onchange="DlnaEnabledSwitch(this.form);">
			<option value="0">Disable</option>
			<option value="1">Enable</option>
			</select>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
		<input type="button" style="{width:80px;}" value="Start" name="dlnastart" onClick="submit_apply('start')">&nbsp;
		<input type="button" style="{width:80px;}" value="Stop" name="dlnastop" onClick="submit_apply('stop')">&nbsp;
		<input type="button" style="{width:80px;}" value="ReScan" name="dlnarescan" onClick="submit_apply('rescan')"></td>
	</td>
</tr>
<tr>
	<td class="head">DLNA port</td>
	<td><input type=text name="dlnaPort" size=5 maxlength=5 value="<% getCfgGeneral(1, "dlnaPort"); %>" ></td>
</tr>
<tr>
	<td class="head">DLNA DB path</td>
	<td><input type=text name="dlnaDBPath" size=64 maxlength=256 value="<% getCfgGeneral(1, "dlnaDBPath"); %>" ></td>
</tr>
<tr>
	<td class="head">DLNA scan path</td>
	<td><input type=text name="dlna0Path" size=64 maxlength=256 value="<% getCfgGeneral(1, "dlna0Path"); %>" ></td>
</tr>
<tr>
	<td class="head">DLNA scan path</td>
	<td><input type=text name="dlna1Path" size=64 maxlength=256 value="<% getCfgGeneral(1, "dlna1Path"); %>" ></td>
</tr>
<tr>
	<td class="head">DLNA scan path</td>
	<td><input type=text name="dlna2Path" size=64 maxlength=256 value="<% getCfgGeneral(1, "dlna2Path"); %>" ></td>
</tr>
</table>

<table class="buttons">
  <tr>
    <td>
	<input value="/usb/minidlna.asp" name="submit-url" type="hidden">
      <input type="button" class="normal" value="Apply" id="TransApply" onClick="submit_apply('apply')"> &nbsp; &nbsp;
      <input type="button"  class="normal" value="Reset" id="TransReset" onClick="window.location.reload()">
    </td>
  </tr>
</table>
</form>
</tr></td>
</table>
</body>
</html>
