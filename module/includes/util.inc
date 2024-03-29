<?php

function contactbook_create_tabular_format_display($all_contacts) {
	$headers = array(t('Image'), t('Full Name'), t('Number'), t('Email'), t('Action'));
	$data = array();
	
	if(count($all_contacts)){
		foreach($all_contacts as $contact) {
			$con_image = "";
			
			if(isset($contact->contact_image_fid) && !empty($contact->contact_image_fid)){
				$image = file_load($contact->contact_image_fid);
				$con_image = theme('image_style', array('style_name' => 'thumbnail', 'path' => $image->uri));
			}
			else {
				$con_image = contactbook_get_defualt_image_html();
			}
			$del = l(t('Delete'), sprintf("contactbook/%s/contact/%s/delete", 'nojs', $contact->id), array('attributes' => array('class' => array('use-ajax'))));
			$edit = l(t('Edit'), sprintf("contactbook/%s/contact/%s/edit", 'nojs', $contact->id), array('attributes' => array('class' => array('use-ajax'))));
			$data[] =  array($con_image, $contact->full_name, '+'.$contact->calling_code .'-'. $contact->phone_number, $contact->email, $edit. ' / ' .$del);
		}
	}
	else {
		$data[] = array(array('data' => t('No Contacts Were Found.'), 'colspan' => '5', 'style' => 'text-align:center; color:#F8A034; font-weight: bold;
    height: 50px')); 
	}
	
	$table_output = theme('table', array('header' => $headers, 'rows' => $data));
	$table_output .= theme('pager');
	
	return $table_output;
	
}

function contactbook_get_defualt_image_html() {
	$module_path = drupal_get_path('module', 'contactbook');	
	$path = $module_path . '/images/default_profile.jpg';
	$image_path = file_create_url($path);
	$con_image = sprintf('<img alt="" src="%s" height="75" width="100">', $image_path);
	return $con_image;
}