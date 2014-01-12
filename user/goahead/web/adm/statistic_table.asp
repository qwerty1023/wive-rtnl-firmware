<table class="form">
<tbody>

<!-- ==============  MEMORY and CPU  =============== -->
<tr>
	<td class="title" colspan="6" id="statisticMMCPU">Memory and CPU</td>
</tr>
<tr>
	<td class="head" id="statisticMMTotal" colspan="3">Memory total: </td>
	<td colspan="3"><% getMemTotalASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticMMLeft" colspan="3">Memory left: </td>
	<td colspan="3"><% getMemLeftASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticCpuUse" colspan="3">CPU usage: </td>
	<td colspan="3"><% getCpuUsageASP(); %></td>
</tr>
<!-- =================  WAN/LAN  ================== -->
<tr>
	<td class="title" colspan="6" id="statisticWANLAN">WAN/LAN</td>
</tr>
<tr>
	<td class="head" id="statisticWANRxPkt" colspan="3">WAN Rx packets: </td>
	<td colspan="3"> <% getWANRxPacketASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticWANRxBytes" colspan="3">WAN Rx bytes: </td>
	<td colspan="3"> <% getWANRxByteASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticWANTxPkt" colspan="3">WAN Tx packets: </td>
	<td colspan="3"> <% getWANTxPacketASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticWANTxBytes" colspan="3">WAN Tx bytes: </td>
	<td colspan="3"> <% getWANTxByteASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticLANRxPkt" colspan="3">LAN Rx packets: &nbsp; &nbsp; &nbsp; &nbsp;</td>
	<td colspan="3"> <% getLANRxPacketASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticLANRxBytes" colspan="3">LAN Rx bytes: </td>
	<td colspan="3"> <% getLANRxByteASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticLANTxPkt" colspan="3">LAN Tx packets: </td>
	<td colspan="3"> <% getLANTxPacketASP(); %></td>
</tr>
<tr>
	<td class="head" id="statisticLANTxBytes" colspan="3">LAN Tx bytes: </td>
	<td colspan="3"> <% getLANTxByteASP(); %></td>
</tr>

<!-- =================  ALL  ================= -->
<tr>
	<td class="title" colspan="6" id="statisticAllIF">All interfaces</td>
<tr>
<tr>
	<th colspan="2">Name</th>
	<th>Rx Packets</th>
	<th>Rx Bytes</th>
	<th>Tx Packets</th>
	<th>Tx Bytes</th>
</tr>
<% getAllNICStatisticASP(); %>
</script>

</tbody>
</table>
