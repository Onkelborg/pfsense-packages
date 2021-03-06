<?php

global $priv_list;

$priv_list['page-services-snort'] = array();
$priv_list['page-services-snort']['name'] = "WebCfg - Services: Snort package.";
$priv_list['page-services-snort']['descr'] = "Allow access to Snort package gui";
$priv_list['page-services-snort']['match'] = array();
$priv_list['page-services-snort']['match'][] = "snort/snort_alerts.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_barnyard.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_blocked.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_check_for_rule_updates.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_define_servers.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_download_rules.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_download_updates.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_edit_hat_data.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_frag3_engine.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_ftp_client_engine.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_ftp_server_engine.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_httpinspect_engine.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_import_aliases.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_interfaces.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_interfaces_edit.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_interfaces_global.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_interfaces_suppress.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_interfaces_suppress_edit.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_passlist.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_passlist_edit.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_list_view.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_migrate_config.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_post_install.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_preprocessors.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_rules.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_rules_edit.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_rules_flowbits.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_rulesets.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_select_alias.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_stream5_engine.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_ip_list_mgmt.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_ip_reputation.php*";
$priv_list['page-services-snort']['match'][] = "snort/snort_iprep_list_browser.php*";
$priv_list['page-services-snort']['match'][] = "widgets/javascript/snort_alerts.js*";
$priv_list['page-services-snort']['match'][] = "widgets/include/widget-snort.inc*";
$priv_list['page-services-snort']['match'][] = "widgets/widgets/snort_alerts.widget.php*";
$priv_list['page-services-snort']['match'][] = "pkg_edit.php?xml=snort_sync.xml*";
$priv_list['page-services-snort']['match'][] = "pkg_edit.php?xml=snort/snort.xml*";
$priv_list['page-services-snort']['match'][] = "snort/snort_check_cron_misc.inc*";
$priv_list['page-services-snort']['match'][] = "snort/snort.inc*";
?>