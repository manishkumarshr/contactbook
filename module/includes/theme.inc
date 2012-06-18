<?php

function template_preprocess_contactbook_my_contact_book_form(&$variables) {
  $variables['contactbook_my_contact_book_form'] = array();
  $hidden = array();

  foreach (element_children($variables['form']) as $key) {
    $type = (array_key_exists('#type', $variables['form'][$key])) ? $variables['form'][$key]['#type'] : '';
    if ($type == 'hidden' || $type == 'token') {
      $hidden[] = drupal_render($variables['form'][$key]);
    } else {
      $variables['contactbook_my_contact_book_form'][$key] = drupal_render($variables['form'][$key]);
    }
  }
  $variables['contactbook_my_contact_book_form']['hidden'] = implode($hidden);
  $variables['contactbook_my_contact_book_form_form'] = implode($variables['contactbook_my_contact_book_form']);
}