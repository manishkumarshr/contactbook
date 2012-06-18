<?php

/**
 * Implement hook_help().
 */
function contactbook_help($path, $arg) {
  switch ($path) {
    case 'admin/help#contactbook':
      return '<p>' . t('A module that helps managing the contacts on an interface, you can store unlimited contacts here and can edit / delete them any time. Go the the path YOURSITE/my-contact-book') . '</p>';
      break;
  }
}

/**
 * Implements hook_init().
 */
function contactbook_init() {
	$path = drupal_get_path("module", "contactbook");
  drupal_add_css("$path/css/contactbook.css");	
	drupal_add_js("$path/js/contactbook.js");
	
	drupal_add_js("misc/jquery.form.js");
	drupal_add_library('system','drupal.ajax');	
}

/**
 * Implementation of hook_permission().
 */
function contactbook_permission() {
  return array(
      'manage contactbook' => array(
          'title' => t('Manage Contact Book'),
      ),
  );
}

/**
 * Implementation of hook_menu().
 */
function contactbook_menu() {

	$items = array();

  $items['admin/config/contactbook'] = array(
		'title' => 'Contact Book',
		'position' => 'right',
		'page callback' => 'system_admin_menu_block_page',
		'access arguments' => array('administer site configuration'),
		'file' => 'system.admin.inc',
		'file path' => drupal_get_path('module', 'system'),
  );
	
	$items['admin/config/contactbook/settings'] = array(
		'title' => 'Contact Book Settings',
		'page callback' => 'drupal_get_form',
		'page arguments' => array('contactbook_admin_settings_form'),
		'access arguments' => array('administer site configuration'),
		'file' => 'includes/admin.inc',
  );
		
	$items['my-contact-book'] = array(
		'title' => 'Contact book - Manage Contacts',
		'page callback' => 'contactbook_manage_form',
		'file' => 'includes/forms.inc',
		'access arguments' => array('manage contactbook'),
  );
	
	$items['contactbook/%/contact/%/delete'] = array(
		'page callback' => 'contactbook_ajax_delete_contact',
		'page arguments' => array(1, 3),
		'access callback' => TRUE,
		'file' => 'includes/ajax.inc',
  );
	
	$items['contactbook/%/contact/%/edit'] = array(
		'page callback' => 'contactbook_ajax_edit_contact',
		'page arguments' => array(1, 3),
		'access callback' => TRUE,
		'file' => 'includes/ajax.inc',
  );
	
  return $items;
}


/**
 * Implementation of hook_theme().
 */
function contactbook_theme() {
  $items = array();

  $items['contact_book_output'] = array(
      'variables' => array('output' => NULL),
      'file' => 'includes/theme.inc',
      'template' => 'templates/contact-book-output',
  );
	
	return $items;
}

/**
 * Implementation of hook_page_alter().
 */
function contactbook_page_alter(&$page) {
	if($_GET['q'] == 'my-contact-book') {
		unset($page['sidebar_first']);
		unset($page['sidebar_second']);
	}
}