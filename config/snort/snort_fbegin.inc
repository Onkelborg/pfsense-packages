<?php

require_once("globals.inc");
require_once("notices.inc");
/* $Id$ */
	function return_ext_menu($section) {
		global $config;
		$htmltext = "";
		if($config['installedpackages']['menu'] <> "") {
			foreach($config['installedpackages']['menu'] as $menuitem) {
				if($menuitem['section'] != $section) continue;
				if($menuitem['url'] <> "") {
				 	$addresswithport = getenv("HTTP_HOST");
					$colonpos = strpos($addresswithport, ":");
					if ($colonpos !== False){
						//my url is actually just the IP address of the pfsense box
						$myurl = substr($addresswithport, 0, $colonpos);
					}
					else
					{
						$myurl = $addresswithport;
					}

					$description = str_replace('$myurl', $myurl, $menuitem['url']);
				} else {
					$description = '/pkg.php?xml=' . $menuitem['configfile'];
				}
				$htmltext .= '<li><a href="' . $description . ' "class="navlnk">' . $menuitem['name'] . '</a></li>' . "\n";
			}
		}
		return $htmltext;
	}




	/* NOTICE ACKNOWLEDGE CODE by Erik Kristensen */
	if ($_REQUEST['noticeaction'] == 'acknowledge') {
		$notice_id = $_REQUEST['noticeid'];
		close_notice($notice_id);
	}
	/**********************************************/
?>

<div id="wrapper">

	<div id="header">
		<div id="header-left"><a href="../index.php" id="status-link"><img src="/themes/<?= $g['theme']; ?>/images/transparent.gif" border="0"></img></a></div>
		<div id="header-right">
			<div class="container">
				<div class="left">webConfigurator</div>
				<div class="right">
<?
				if (are_notices_pending()) {
					$notices = get_notices();

					$requests=array();

					## Get Query Arguments from URL ###
					foreach ($_REQUEST as $key => $value) {
						if ($key != "PHPSESSID")
							$requests[] = $key.'='.$value;
					}
					if(is_array($requests))
						$request_string = implode("&", $requests);

					if(is_array($notices)) {
						foreach ($notices as $key => $value) {
							$date = date("m-d-y H:i:s", $key);
							$noticemsg = str_replace("'", "", $value['notice']);
							$noticemsg = str_replace('"', "", $noticemsg);
							$noticemsg = str_replace("\n", "", $noticemsg);
							$noticemsg = str_replace("<p>", "", $noticemsg);
							$noticemsg = str_replace("<pre>", "", $noticemsg);
							$noticemsg = str_replace("</pre>", "", $noticemsg);
							$noticemsg = str_replace("</p>", "", $noticemsg);
							$noticemsg = str_replace("<br>", "", $noticemsg);
							$extra_args = "";
							if($_GET['xml']) 
								$extraargs="&xml=" . $_GET['xml'];
							if($_POST['xml']) 
								$extraargs="&xml=" . $_POST['xml'];
							if($_GET['id']) 
								$extraargs="&xml=" . $_GET['id'];
							if($_POST['id'])
								$extraargs="&xml=" . $_POST['id'];
							$notice_msgs = '<a href="?noticeaction=acknowledge&noticeid=all' . $extraargs . '">Acknowledge All</a> &nbsp;&nbsp;&nbsp;&nbsp;.:.&nbsp;&nbsp;&nbsp;&nbsp; ';
							if ($value['url']) {
								$notice_msgs .= $date.' - <a href="'.$url.'?'.$request_string.'&noticeaction=acknowledge&noticeid='.$key.'">['.$value['id'].']</a>';
							} else {
								$notice_msgs .= $date.' - <a href="?'.$request_string.'&noticeaction=acknowledge&noticeid='.$key.'">['.$value['id'].']'.$noticemsg.'</a>';
							}
							$notice_msgs .= " &nbsp;&nbsp;&nbsp;&nbsp;.:.&nbsp;&nbsp;&nbsp;&nbsp; ";
						}
					}
?>
					<div id="alerts">
						<script type="text/javascript">
							var content='<div id="marquee-text"><?= $notice_msgs; ?></div>'
						</script>
						<script type="text/javascript" src="/ticker.js"></script>
					</div>
<?
				} else {
?>
					<div id="hostname">
						<? print $config['system']['hostname'] . "." . $config['system']['domain']; ?>
					</div>
<?
				}
?>
				</div>
			</div>
		</div>
	</div> <!-- Header DIV -->
	<div id="content">
		<div id="left">
			<div id="navigation" style="z-index:1000">  
				<ul id="menu">
					<li class="firstdrop">
						<div>System</div>
						<ul class="subdrop">
							<li><a href="/system_advanced.php" class="navlnk">Advanced</a></li>
							<li><a href="/system_firmware.php" class="navlnk">Firmware</a></li>
							<li><a href="/system.php" class="navlnk">General Setup</a></li>
						<?php if($g['platform'] == "pfSense" or $g['platform'] == "nanobsd"): ?>
							<li><a href="/pkg_mgr.php" class="navlnk">Packages</a></li>
						<?php endif; ?>
							<li><a href="/wizard.php?xml=setup_wizard.xml" class="navlnk">Setup wizard</a></li>
							<li><a href="/system_routes.php" class="navlnk">Static routes</a></li>
						</ul>
					</li>
					<li class="drop">
						<div>Interfaces</div>
						<ul class="subdrop">
							<?php if (!isset($config['system']['webgui']['noassigninterfaces'])): ?><li><a href="/interfaces_assign.php" class="navlnks">(assign)</a></li><?php endif; ?>
							<li><a href="/interfaces_wan.php" class="navlnk">WAN</a></li>
							<li><a href="/interfaces_lan.php" class="navlnk">LAN</a></li>
							<?php for ($i = 1; isset($config['interfaces']['opt' . $i]); $i++): if (!isset($config['interfaces']['opt' . $i]['ovpn'])): ?>
							<li><a href="/interfaces_opt.php?index=<?=$i;?>" class="navlnk"><?=htmlspecialchars($config['interfaces']['opt' . $i]['descr']);?></a></li>
							<?php endif; endfor; ?>
							<?php echo return_ext_menu("Interfaces"); ?>
						</ul>
					</li>
<?php
        if($config['system']['shapertype'] <> "m0n0")
          $shaper = "firewall_shaper.php";
        else
          $shaper = "m0n0/firewall_shaper.php";
?>
					<li class="drop">
						<div>Firewall</div>
						<ul class="subdrop">
							<li><a href="/firewall_aliases.php" class="navlnk">Aliases</a></li>
							<li><a href="/firewall_nat.php" class="navlnk">NAT</a></li>
							<li><a href="/firewall_rules.php" class="navlnk">Rules</a></li>
							<li><a href="/firewall_schedule.php" class="navlnk">Schedules</a></li>
							<li><a href="<?=$shaper?>" class="navlnk">Traffic Shaper</a></li>
							<li><a href="/firewall_virtual_ip.php" class="navlnk">Virtual IPs</a></li>
							<?php echo return_ext_menu("Firewall"); ?>
						</ul>
					</li>
					<li class="drop">
						<div>Services</div>
						<ul class="subdrop">
							<li><a href="/services_captiveportal.php" class="navlnk">Captive portal</a></li>
							<li><a href="/services_dnsmasq.php" class="navlnk">DNS forwarder</a></li>
							<li><a href="/services_dhcp_relay.php" class="navlnk">DHCP relay</a></li>
							<li><a href="/services_dhcp.php" class="navlnk">DHCP server</a></li>
							<li><a href="/services_dyndns.php" class="navlnk">Dynamic DNS</a></li>
							<li><a href="/load_balancer_pool.php" class="navlnk">Load Balancer</a></li>
							<li><a href="/pkg_edit.php?xml=olsrd.xml&id=0" class="navlnk">OLSR</a></li>
							<li><a href="/vpn_pppoe.php" class="navlnk">PPPoE Server</a></li>
							<li><a href="/pkg_edit.php?xml=routed/routed.xml&id=0" class="navlnk">RIP</a></li>
							<li><a href="/services_snmp.php" class="navlnk">SNMP</a></li>
							<li><a href="/pkg_edit.php?xml=miniupnpd.xml&id=0" class="navlnk">UPnP</a></li>
							<li><a href="/pkg_edit.php?xml=openntpd.xml&id=0" class="navlnk">OpenNTPD</a></li>
							<li><a href="/services_wol.php" class="navlnk">Wake on LAN</a></li>
							<?php echo return_ext_menu("Services"); ?>
						</ul>
					</li>
					<li class="drop">
						<div>VPN</div>
						<ul class="subdrop">
							<li><a href="/vpn_ipsec.php" class="navlnk">IPsec</a></li>
							<li><a href="/pkg.php?xml=openvpn.xml" class="navlnk">OpenVPN</a></li>
							<li><a href="/vpn_pptp.php" class="navlnk">PPTP</a></li>
							<?php echo return_ext_menu("VPN"); ?>
						</ul>
					</li>
					<li class="drop">
						<div>Status</div>
						<ul class="subdrop">
							<?php if (isset($config['captiveportal']['enable'])): ?>
							<li><a href="/status_captiveportal.php" class="navlnk">Captive portal</a></li>
							<?php endif; ?>
							<li><a href="/carp_status.php" class="navlnk">CARP (failover)</a></li>
							<li><a href="/diag_dhcp_leases.php" class="navlnk">DHCP leases</a></li>
							<li><a href="/status_filter_reload.php" class="navlnk">Filter Reload Status</a></li>
							<li><a href="/status_interfaces.php" class="navlnk">Interfaces</a></li>
							<li><a href="/diag_ipsec.php" class="navlnk">IPsec</a></li>
							<li><a href="/status_slbd_pool.php" class="navlnk">Load Balancer</a></li>
							<?php if($g['platform'] == "pfSense"): ?>
							<li><a href="/diag_pkglogs.php" class="navlnk">Package logs</a></li>
							<?php endif; ?>
							<li><a href="/status_queues.php" class="navlnk">Queues</a></li>
							<li><a href="/status_rrd_graph.php" class="navlnk">RRD Graphs</a></li>
							<li><a href="/status_services.php" class="navlnk">Services</a></li>
							<li><a href="/index.php" class="navlnk">System</a></li>
							<li><a href="/diag_logs.php" class="navlnk">System logs</a></li>
							<li><a href="/status_graph.php?if=wan" class="navlnk">Traffic graph</a></li>
							<li><a href="/status_upnp.php" class="navlnk">UPnP</a></li>
							<?php $i = 0; $ifdescrs = array();
							if (is_array($config['interfaces']['wan']['wireless']) &&
								preg_match($g['wireless_regex'], $config['interfaces']['wan']['if']))
									$ifdescrs['wan'] = 'WAN';
							if (is_array($config['interfaces']['lan']['wireless']) &&
								preg_match($g['wireless_regex'], $config['interfaces']['lan']['if']))
									$ifdescrs['lan'] = 'LAN';
							for ($j = 1; isset($config['interfaces']['opt' . $j]); $j++) {
								if (is_array($config['interfaces']['opt' . $j]['wireless']) &&
									isset($config['interfaces']['opt' . $j]['enable']) &&
									 preg_match($g['wireless_regex'], $config['interfaces']['opt' . $j]['if']))
										$ifdescrs['opt' . $j] = $config['interfaces']['opt' . $j]['descr'];
							}
							if (count($ifdescrs) > 0): ?>
							<li><a href="/status_wireless.php" class="navlnk">Wireless</a></li>
							<?php endif; ?>
							<?php echo return_ext_menu("Status"); ?>
						</ul>
					</li>
					<li class="lastdrop">
						<div>Diagnostics</div>
						<ul id="diag" class="subdrop">
							<li><a href="/diag_arp.php" class="navlnk">ARP Tables</a></li>
							<li><a href="/diag_backup.php" class="navlnk">Backup/Restore</a></li>
							<li><a href="/exec.php" class="navlnk">Command Prompt</a></li>
							<li><a href="/edit.php" class="navlnk">Edit File</a></li>
							<li><a href="/diag_defaults.php" class="navlnk">Factory defaults </a></li>
							<li><a href="/halt.php" class="navlnk">Halt system</a></li>
<?php if($g['platform']=="nanobsd"): ?>
							<li><a href="/diag_nanobsd.php" class="navlnk">NanoBSD</a></li>	
<?php endif; ?>
							<li><a href="/diag_ping.php" class="navlnk">Ping</a></li>
							<li><a href="/reboot.php" class="navlnk">Reboot system</a></li>
							<li><a href="/diag_routes.php" class="navlnk">Routes</a></li>
							<li><a href="/diag_dump_states.php" class="navlnk">States</a></li>
							<li><a href="/diag_traceroute.php" class="navlnk">Traceroute</a></li>
							<li><a href="/diag_packet_capture.php" class="navlnk">Packet Capture</a></li>
							<?php echo return_ext_menu("Diagnostics"); ?>
							<?php if(isset($config['system']['developer'])): ?>
							<li><hr width="80%"/></li>
							<li><a href="/restart_httpd.php" class="navlnk">Restart HTTPD</a></li>
							<?php endif; ?>
						</ul>
					</li>
				</ul>
			</div>

		</div> <!-- Left DIV -->

		<div id="right">


<?php
	/* display a top alert bar if need be */
	$need_alert_display = false;
	$found_notices = are_notices_pending();
	if($found_notices == true) {
		$notices = get_notices();
		if(!$notices) {
			$need_alert_display = true;
			$display_text = print_notices() . "<br>";
		}
	}
	if($need_alert_display == true) {
                echo "<div style=\"background-color:#000000\" id=\"roundalert\">";
                echo "<table>";
		echo "<tr><td><font color=\"#ffffff\">";
		echo "&nbsp;&nbsp;<img align=\"middle\" src=\"/top_notification.gif\">&nbsp;&nbsp;&nbsp;";
		echo $display_text;
		echo "</td>";
		echo "</tr>";
		echo "</table>";
		echo "</div>";
	}

?>