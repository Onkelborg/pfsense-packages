<?php
/* $Id$ */
/*
	snort_import_aliases.php
	Copyright (C) 2013 Bill Meeks
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice,
	   this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
	AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

require("guiconfig.inc");
require_once("functions.inc");
require_once("/usr/local/pkg/snort/snort.inc");

// Retrieve any passed QUERY STRING or POST variables
$id = $_GET['id'];
$eng = $_GET['eng'];
if (isset($_POST['id']))
	$id = $_POST['id'];
if (isset($_POST['eng']))
	$eng = $_POST['eng'];

// Make sure we have a valid rule ID and ENGINE name, or
// else bail out to top-level menu. 
if (is_null($id) || is_null($eng)) {
 	header("Location: /snort/snort_interfaces.php");
	exit;
}

// Used to track if any selectable Aliases are found
$selectablealias = false;

// Initialize required array variables as necessary
if (!is_array($config['aliases']['alias']))
	$config['aliases']['alias'] = array();
$a_aliases = $config['aliases']['alias'];
if (!is_array($config['installedpackages']['snortglobal']['rule']))
	$config['installedpackages']['snortglobal']['rule'] = array();

// The $eng variable points to the specific Snort config section
// engine we are importing values into.  Initialize the config.xml
// array if necessary.
if (!is_array($config['installedpackages']['snortglobal']['rule'][$id][$eng]['item']))
	$config['installedpackages']['snortglobal']['rule'][$id][$eng]['item'] = array();

// Initialize a pointer to the Snort config section engine we are
// importing values into.
$a_nat = &$config['installedpackages']['snortglobal']['rule'][$id][$eng]['item'];

// Build a lookup array of currently used engine 'bind_to' Aliases 
// so we can screen matching Alias names from the list.
$used = array();
foreach ($a_nat as $v)
	$used[$v['bind_to']] = true;

// Construct the correct return anchor string based on the Snort config section
// engine we were called with.  This lets us return to the page and section
// we were called from.  Also set the flag for those engines which accept
// multiple IP addresses for the "bind_to" parameter.
switch ($eng) {
	case "frag3_engine":
		$anchor = "#frag3_row";
		$multi_ip = true;
		$title = "Frag3 Engine";
		break;
	case "http_inspect_engine":
		$anchor = "#httpinspect_row";
		$multi_ip = true;
		$title = "HTTP_Inspect Engine";
		break;
	case "stream5_tcp_engine":
		$anchor = "#stream5_row";
		$multi_ip = true;
		$title = "Stream5 TCP Engine";
		break;
	case "ftp_server_engine":
		$anchor = "#ftp_telnet_row";
		$multi_ip = false;
		$title = "FTP Server Engine";
		break;
	case "ftp_client_engine":
		$anchor = "#ftp_telnet_row";
		$multi_ip = false;
		$title = "FTP Client Engine";
		break;
	default:
		$anchor = "";
}

if ($_POST['cancel']) {
	header("Location: /snort/snort_preprocessors.php?id={$id}{$anchor}");
	exit;
}

if ($_POST['save']) {

	// Define default engine configurations for each of the supported engines.

	$def_frag3 = array( "name" => "", "bind_to" => "", "policy" => "bsd", 
		  	    "timeout" => 60, "min_ttl" => 1, "detect_anomalies" => "on", 
			    "overlap_limit" => 0, "min_frag_len" => 0 );

	$def_ftp_server = array( "name" => "", "bind_to" => "", "ports" => "default", 
		   		 "telnet_cmds" => "no", "ignore_telnet_erase_cmds" => "yes", 
				 "ignore_data_chan" => "no", "def_max_param_len" => 100 );

	$def_ftp_client = array( "name" => "", "bind_to" => "", "max_resp_len" => 256, 
			         "telnet_cmds" => "no", "ignore_telnet_erase_cmds" => "yes", 
			         "bounce" => "yes", "bounce_to_net" => "", "bounce_to_port" => "" );

	$def_http_inspect = array( "name" => "", "bind_to" => "", "server_profile" => "all", "enable_xff" => "off", 
				   "log_uri" => "off", "log_hostname" => "off", "server_flow_depth" => 65535, "enable_cookie" => "on", 
				   "client_flow_depth" => 1460, "extended_response_inspection" => "on", "no_alerts" => "off", 
				   "unlimited_decompress" => "on", "inspect_gzip" => "on", "normalize_cookies" =>"on", "normalize_headers" => "on", 
				   "normalize_utf" => "on", "normalize_javascript" => "on", "allow_proxy_use" => "off", "inspect_uri_only" => "off", 
				   "max_javascript_whitespaces" => 200, "post_depth" => -1, "max_headers" => 0, "max_spaces" => 0, 
				   "max_header_length" => 0, "ports" => "default" );

	$def_stream5 = array(	"name" => "", "bind_to" => "", "policy" => "bsd", "timeout" => 30, 
				"max_queued_bytes" => 1048576, "detect_anomalies" => "off", "overlap_limit" => 0, 
				"max_queued_segs" => 2621, "require_3whs" => "off", "startup_3whs_timeout" => 0, 
				"no_reassemble_async" => "off", "dont_store_lg_pkts" => "off", "max_window" => 0, 
				"use_static_footprint_sizes" => "off", "check_session_hijacking" => "off", "ports_client" => "default", 
				"ports_both" => "default", "ports_server" => "none" );

	// Figure out which engine type we are importing and set up default engine array
	$engine = array();
	switch ($eng) {
		case "frag3_engine":
			$engine = $def_frag3;
			break;
		case "http_inspect_engine":
			$engine = $def_http_inspect;
			break;
		case "stream5_tcp_engine":
			$engine = $def_stream5;
			break;
		case "ftp_server_engine":
			$engine = $def_ftp_server;
			break;
		case "ftp_client_engine":
			$engine = $def_ftp_client;
			break;
		default:
			$engine = "";
			$input_errors[] = gettext("Invalid ENGINE TYPE passed in query string.  Aborting operation.");
	}

	// See if anything was checked to import
	if (is_array($_POST['toimport']) && count($_POST['toimport']) > 0) {
		foreach ($_POST['toimport'] as $item) {
			$engine['name'] = strtolower($item);
			$engine['bind_to'] = $item;
			$a_nat[] = $engine;
		}
	}
	else
		$input_errors[] = gettext("No entries were selected for import.  Please select one or more Aliases for import and click SAVE.");

	// if no errors, write new entry to conf
	if (!$input_errors) {
		// Reorder the engine array to ensure the 
		// 'bind_to=all' entry is at the bottom if 
		// the array contains more than one entry.
		if (count($a_nat) > 1) {
			$i = -1;
			foreach ($a_nat as $f => $v) {
				if ($v['bind_to'] == "all") {
					$i = $f;
					break;
				}
			}
			// Only relocate the entry if we 
			// found it, and it's not already 
			// at the end.
			if ($i > -1 && ($i < (count($a_nat) - 1))) {
				$tmp = $a_nat[$i];
				unset($a_nat[$i]);
				$a_nat[] = $tmp;
			}
		}

		// Now write the new engine array to conf and return
		write_config();

		header("Location: /snort/snort_preprocessors.php?id={$id}{$anchor}");
		exit;
	}
}

$pgtitle = gettext("Snort: Import Host/Network Alias for {$title}");
include("head.inc");

?>

<body link="#0000CC" vlink="#0000CC" alink="#0000CC">
<?php include("fbegin.inc"); ?>
<form action="snort_import_aliases.php" method="post">
<input type="hidden" name="id" value="<?=$id;?>">
<input type="hidden" name="eng" value="<?=$eng;?>">
<?php if ($input_errors) print_input_errors($input_errors); ?>
<div id="boxarea">
<table width="100%" border="0" cellpadding="0" cellspacing="0">
<tr>
	<td class="tabcont"><strong><?=gettext("Select one or more Aliases to use as {$title} targets from the list below.");?></strong><br/>
	</td>
</tr>
<tr>
	<td class="tabcont">
		<table id="sortabletable1" style="table-layout: fixed;" class="sortable" width="100%" border="0" cellpadding="0" cellspacing="0">
			<colgroup>
				<col width="5%" align="center">
				<col width="25%" align="left" axis="string">
				<col width="35%" align="left" axis="string">
				<col width="35%" align="left" axis="string">
			</colgroup>
			<thead>
			   <tr>
				<th class="listhdrr"></th>
				<th class="listhdrr" axis="string"><?=gettext("Alias Name"); ?></th>
				<th class="listhdrr" axis="string"><?=gettext("Values"); ?></th>
				<th class="listhdrr" axis="string"><?=gettext("Description"); ?></th>
			   </tr>
			</thead>
		<tbody>
		  <?php $i = 0; foreach ($a_aliases as $alias): ?>
			<?php if ($alias['type'] <> "host" && $alias['type'] <> "network")
				continue;
			      if (isset($used[$alias['name']]))
				continue;
			      if (!$multi_ip && !snort_is_single_addr_alias($alias['name'])) {
				$textss = "<span class=\"gray\">";
				$textse = "</span>";
				$disable = true;
			        $tooltip = gettext("Aliases resolving to multiple addresses cannot be used with the '{$eng}'.");
			      }
			      elseif (trim(filter_expand_alias($alias['name'])) == "") {
				$textss = "<span class=\"gray\">";
				$textse = "</span>";
				$disable = true;
			        $tooltip = gettext("Aliases representing a FQDN host cannot be used in Snort preprocessor configurations.");
			      }
			      else {
				$textss = "";
				$textse = "";
				$disable = "";
				$selectablealias = true;
			        $tooltip = gettext("Selected entries will be imported. Click to toggle selection of this entry.");
			      }
			?>
			<?php if ($disable): ?>
			<tr title="<?=$tooltip;?>">
			  <td class="listlr" align="center"><img src="../themes/<?=$g['theme'];?>/images/icons/icon_block_d.gif" width="11" height"11" border="0"/>
			<?php else: ?>
			<tr>
			  <td class="listlr" align="center"><input type="checkbox" name="toimport[]" value="<?=htmlspecialchars($alias['name']);?>" title="<?=$tooltip;?>"/></td>
			<?php endif; ?>
			  <td class="listr" align="left"><?=$textss . htmlspecialchars($alias['name']) . $textse;?></td>
			  <td class="listr" align="left">
			      <?php
				$tmpaddr = explode(" ", $alias['address']);
				$addresses = implode(", ", array_slice($tmpaddr, 0, 10));
				echo "{$textss}{$addresses}{$textse}";
				if(count($tmpaddr) > 10) {
					echo "...";
				}
			    ?>
			  </td>
			  <td class="listbg" align="left">
			    <?=$textss . htmlspecialchars($alias['descr']) . $textse;?>&nbsp;
			  </td>
			</tr>
		  <?php $i++; endforeach; ?>
		</table>
	</td>
</tr>
<?php if (!$selectablealias): ?>
<tr>
	<td class="tabcont" align="center"><b><?php echo gettext("There are currently no defined Aliases eligible for import.");?></b></td>
</tr>
<tr>
	<td class="tabcont" align="center">
	<input type="Submit" name="cancel" value="Cancel" id="cancel" class="formbtn" title="<?=gettext("Cancel import operation and return");?>"/>
	</td>
</tr>
<?php else: ?>
<tr>
	<td class="tabcont" align="center">
	<input type="Submit" name="save" value="Save" id="save" class="formbtn" title="<?=gettext("Import selected item and return");?>"/>&nbsp;&nbsp;&nbsp;
	<input type="Submit" name="cancel" value="Cancel" id="cancel" class="formbtn" title="<?=gettext("Cancel import operation and return");?>"/>
	</td>
</tr>
<?php endif; ?>
<tr>
	<td class="tabcont">
	<span class="vexpl"><span class="red"><strong><?=gettext("Note:"); ?><br></strong></span><?=gettext("Fully-Qualified Domain Name (FQDN) host Aliases cannot be used as Snort configuration parameters.  Aliases resolving to a single FQDN value are disabled in the list above.  In the case of nested Aliases where one or more of the nested values is a FQDN host, the FQDN host will not be included in the {$title} configuration.");?></span>
	</td>
</tr>
</table>
</div>
</form>
<?php include("fend.inc"); ?>
</body>
</html>