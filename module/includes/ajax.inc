<?php

/**
 * Ajax call back function for deleting a contact.
 */
function contactbook_ajax_delete_contact($type, $contact_id){
	$type = 'ajax'; 
 
  if ($type == 'ajax') {
		if(contactbook_delete_contact_by_id($contact_id)){
			$msg = "Contact has been deleted successfully";
		}else{
			$msg = "Could not delete this contact";
		}		
		
		$all_contacts = contactbook_get_all_saved_contact(NULL);
		
		module_load_include('inc', 'contactbook', 'includes/util');
		$table_output = contactbook_create_tabular_format_display($all_contacts);
		
		$commands[] = ajax_command_replace('#contactbook_contact_list_wrapper', '<div id="contactbook_contact_list_wrapper">'.$table_output.'</div>');
		
		$commands[] = ajax_command_alert($msg);
		$ret = array('#type' => 'ajax', '#commands' => $commands);
		ajax_deliver($ret);
  }
  else {
    return '';
  }
  
}

/**
 * Ajax call back function for editing a functioin.
 */
function contactbook_ajax_edit_contact($type, $contact_id) {
	$type = 'ajax'; 
 
  if ($type == 'ajax') {
		
		$contact = contactbook_get_contact_by_id($contact_id);		
		
		$commands[] = ajax_command_invoke('#edit-adding-contact', 'removeClass', array('collapsed'));		
		$commands[] = ajax_command_invoke('#edit-adding-contact .fieldset-wrapper', 'attr', array('style', 'display:block'));
		$commands[] = ajax_command_invoke('input[name="contact_id"]', 'attr', array('value', $contact->id));
		$commands[] = ajax_command_invoke('#edit-full-name', 'attr', array('value', $contact->full_name));
		$commands[] = ajax_command_invoke('#edit-phone-number', 'attr', array('value', $contact->phone_number));
		$commands[] = ajax_command_invoke('#edit-email', 'attr', array('value', $contact->email));
    
		
		$image = file_load($contact->contact_image_fid);
		if($image){
			$commands[] = ajax_command_invoke('#pic-div', 'removeClass', array('default-pic-div'));
			$commands[] = ajax_command_replace('#contact-image-div', '<div id="contact-image-div">'.theme('image_style', array('style_name' => 'thumbnail', 'path' => $image->uri)).'<div class="description">Uploading New Image will Replace This Image</div></div>');
		}else {
			module_load_include('inc', 'contactbook', 'includes/util');
			$con_image = contactbook_get_defualt_image_html();
			$commands[] = ajax_command_replace('#contact-image-div', '<div id="contact-image-div">'.$con_image.'<div class="description">Uploading New Image will Replace This Image</div></div>');
			$commands[] = ajax_command_invoke('#pic-div', 'addClass', array('default-pic-div'));
		}
		$ret = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($ret);		

  }
  else {
    return '';
  }
}