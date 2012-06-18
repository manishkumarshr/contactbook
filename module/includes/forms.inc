<?php
/**
 * The Main form display.
 */
function contactbook_manage_form(){
	$form1 = drupal_get_form('contactbook_contact_address_book_form');
	$form2 = drupal_get_form('contactbook_search_show_contacts_form');
	
	$output = drupal_render($form1) . '</BR>' . drupal_render($form2);

	$content =  theme('contact_book_output', array('output' => $output));
	
	return $content;
}
/**
 * Adding a contact form.
 */
function contactbook_contact_address_book_form($form, &$form_state) {
	$form = array();
	
	$form['adding_contact'] = array(
      '#type' => 'fieldset',
      '#title' => t("Add New Contact"),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );
	
	$form['adding_contact']['contact_id'] = array(
		'#type' => 'hidden',
	);	
	$form['adding_contact']['full_name'] = array(
		'#title' => t('Fulll Name'),
		'#type' => 'textfield',
		'#size' => 40,
		'#required' => TRUE,
	);
	$form['adding_contact']['calling_code'] = array(
		'#title' => t('ISD Code'),
		'#type' => 'textfield',
		'#size' => 5,
		'#required' => TRUE,
		'#default_value' => variable_get('default_county_phone_code'),
	);
	$form['adding_contact']['phone_number'] = array(
		'#title' => t('Phone Number'),
		'#type' => 'textfield',
		'#size' => 25,
		'#required' => TRUE,
	);
	$form['adding_contact']['email'] = array(
		'#title' => t('Email'),
		'#description' => t('Provide Valid Email Id'),
		'#type' => 'textfield',
		'#size' => 40,
	);
	
	module_load_include('inc', 'contactbook', 'includes/util');
	$con_image = contactbook_get_defualt_image_html();
	
	$form['adding_contact']['current_contact_image'] = array(
		'#type' => 'item',
		'#title' => 'Current Image',
		'#markup' => '<div id="contact-image-div">' . $con_image . '</div>',
	);
  
	$form['adding_contact']['remove_image_use_default'] = array(
		'#type' => 'checkbox',
		'#prefix' => '<div id="pic-div" class="default-pic-div">',
		'#suffix' => '</div>',
		'#title' => 'Remove this image and use default',
		'#default_value' => false,
	);
	
	//$del = l(t('Upload'), sprintf("contactbook/%s/contact/%s/delete", 'nojs', 'manish'), array('attributes' => array('class' => array('use-ajax'))));
	$form['adding_contact']['contact_image'] = array(
		'#title' => t('Upload Contact Image'),
		'#type' => 'file',
		//'#suffix' => $del,
		'#size' => 30,
	);
	
	$form['adding_contact']['submit'] = array(
		'#type' => 'submit',
		'#prefix' => '<input class="form-submit" type="reset" value="Cancel" onClick="location.reload();"/>',
    '#value' => t('Save'),
	);
		
	return $form;
	
}

/**
 * contactbook_contact_address_book_form validaiotn :  Email validation.
 */
function contactbook_contact_address_book_form_validate($form, &$form_state) {
	$v = $form_state['values'];
	
	if (!filter_var($v['email'], FILTER_VALIDATE_EMAIL)) {
		form_set_error('email', 'Invalid Email Id');
	}
}

/**
 * contactbook_contact_address_book_form submit handler.
 */
function contactbook_contact_address_book_form_submit($form, &$form_state) {
	$v = $form_state['values'];
	$object = new stdClass();
	if($v['contact_id'] != "") {
		$keys = array('id');
		$object->id = $v['contact_id'];
		$msg = 'updated';
	}
	else {
		$keys = array();
		$msg = 'saved';
	}
	
	$dir = 'public://contactbook/contact-images/';
  file_prepare_directory($dir, FILE_CREATE_DIRECTORY);

  $validators = array(
		'file_validate_extensions' => array('jpg jpeg png gif'),
  );
  $file = file_save_upload('contact_image', $validators, $dir, FILE_EXISTS_RENAME);
	
	$object->full_name = $v['full_name'];
	$object->phone_number = $v['phone_number'];
	$object->email = $v['email'];
	$object->calling_code = $v['calling_code'];
	
	if ($file) {
    // Very important to do this or file will be deleted on cron
		$file->status = FILE_STATUS_PERMANENT;
		
		$object->contact_image_fid = $file->fid;
  }
	
	if(($v['remove_image_use_default']) && $v['contact_id']) {
		$object->contact_image_fid = "";
	}
	
	if($v['op'] == 'Save') {
		drupal_write_record('contact_address_book', $object, $keys);
		drupal_set_message(t('contact :name has been :action in your phone book', array(':name' => $object->full_name, ':action' => $msg)));
	}
}

/**
 * Show and search contact form.
 */
function contactbook_search_show_contacts_form($form, &$form_state) {
  $form['search_contact'] = array(
      '#type' => 'textfield',
      '#title' => t('Type To Search'),
			'#size' => 20,
			'#ajax' => array(
				'callback' => 'contactbook_reload_contact_list',
				'wrapper' => 'contactbook_contact_list_wrapper',
				'event' => 'keyup',
			),
  );
	
	if(!empty($form_state['values']['search_contact'])) {
		$all_contacts = contactbook_get_all_saved_contact($form_state['values']['search_contact']);
	}
	else {	
		$all_contacts = contactbook_get_all_saved_contact(NULL);		// contactbook.module
	}
	
	module_load_include('inc', 'contactbook', 'includes/util');
	$table_output = contactbook_create_tabular_format_display($all_contacts);
	
	$form['contact_display'] = array(
		'#type' => 'markup',
		'#prefix' => '<div id="contactbook_contact_list_wrapper">',
		'#suffix' => '</div>',
		'#markup' => $table_output,
  );
		
  return $form;
}

/**
 * Ajax call back function when a search is made.
 */
function contactbook_reload_contact_list($form, $form_state) {
	return $form['contact_display'];
}