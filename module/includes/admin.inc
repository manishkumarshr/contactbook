<?php

function contactbook_admin_settings_form() {
	$form = array();
	$form['default_county_phone_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Default Country Phone Code'),
		'#default_value' => variable_get('default_county_phone_code', ''),
  );

  return system_settings_form($form);
}