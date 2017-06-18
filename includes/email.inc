<?
/*
 * Created on May 16, 2017
 * Created by Charlotta Beavers
 * lottie@openflows.com
 * www.openflows.com
 * 
 * Created for Open Signal PDX (PCM)
 */


/**
  * Send a notification to the producer after s/he submits a sereies request
  */
function cm_series_request_submitted_producer_notification_send($node) {
  $series_request = cm_series_request_get_series_request($node);
  if (!isset($series_request)) {
    return;
  } 

  $roles = array(CM_SERIES_REQUEST_SITE_ADMIN_ROLE);
  $is_admin = cm_series_request_user_has_role($roles) ?
      TRUE : FALSE;
  
  // prep and send the email
  if (!$is_admin) {  
    $to = $series_request['producers_email'];    
    $header_bcc = 'frontdesk@opensignalpdx.org,playback@opensignalpdx.org';   
  }
  else {
    // FOR TESTING - SET EMAIL
    global $user;
    $to = $user->mail;      
  }
   
  $from = variable_get('site_mail');
  if (!$from) {
    drupal_set_message("Error: Site wide email address is not set: ".
          "cm_series_request_submitted_producer_notification_send", 'error');
  }

  $language = language_default();  
  $subject_variables = array(
      '!project_title' => $series_request['project_title'], 
  );
  $subject = t("!project_title : Series Request Submitted", $subject_variables, 
                  array(
                  'langcode' => $language->language,
                  ) 
              );
     
  $message = 
      cm_series_request_submitted_producer_notification_body($series_request);

  $headers = 'From: '.$from . "\r\n" .
    'Reply-To: ' .$from . "\r\n" .
    'X-Mailer: PHP/' . phpversion() . "\r\n" .
    'Bcc: ' .$header_bbc;
  
  $from = variable_get('site_mail');

  mail($to, $subject, $message, $headers);
  drupal_set_message("Email sent to ".$to);  
  
  // no need to check for failure - drupal_mail $message['result'] contains
  // the success indicator of the email, failure being already written to the
  // watchdog; success means nothing more than the message being accepted at
  // php-level, which still doesn't guarantee it to be delivered
  //$mail = drupal_mail($module, $key, $to, $language, $mail_params, $from, $send);
 
}

/**
  * Producter notitifcation sent when series request is created/submitted
  *
  */
function cm_series_request_submitted_producer_notification_body (
                                                        $series_request) {
  $node_url = url('node/' . $series_request['nid'], array('absolute' => TRUE));
  $alias_url = drupal_get_path_alias($node_url);
  
  $body_variables = array(
    '!producers_name' => $series_request['producers_name'],
    '!producers_email' => $series_request['producers_email'],
    '!created' => $series_request['created'],
    '!season_start' => $series_request['season_start'],
    '!season_start_string' => $series_request['season_start_string'],
    '!season_end_string' => $series_request['season_end_string'],
    '!season_title' => $series_request['season_name'],    
    '!project_nid' => $series_request['project_nid'],
    '!project_title' => $series_request['project_title'],
    '!project_type' => $series_request['project_type'],    
    '!series_title' => $series_request['title'],
    '!series_request_frequency' => 
            $series_request['series_request_frequency'],
    '!est_completion_date' => $series_request['est_completion_date'],    
    '!keep_current_schedule' => $series_request['keep_current_schedule'],
    '!pub_producer_name' => $series_request['pub_producer_name'],
    '!pub_contact_phone' => $series_request['pub_contact_phone'],
    '!pub_contact_email' => $series_request['pub_contact_email'],
    '!pub_website' => $series_request['pub_website'],
    '!pbcore_languages' => $series_request['pbcore_languages'],
    '!pbcore_genres' => $series_request['pbcore_genres'],    
    '!short_description' => $series_request['short_description'],
    '!description_body' => $series_request['description_body'],
    '!live' => $series_request['live'],
    '!objectionable_content' => $series_request['objectionable_content'],
    '!call_in' => $series_request['call_in'],        
    '!closed_captions' => $series_request['closed_captions'],
    '!filler' => $series_request['filler'],
    '!filler_end_date' => $series_request['filler_end_date'],
    '!runtime_default' => $series_request['runtime_default'],
    '!playback_preferences' => $series_request['playback_prefs_comments'],
    '!show_title' => $series_request['show_title'],
    '!url' => $alias_url,
  );    
    
  $body = 
    t("Your project submission has been received for !project_title and is ".
      "pending review by a Open Signal staff. The review process and ".
      "scheduling can take up to three weeks.\n" .
      "\n",
      $body_variables);

  $body .= variable_get('series_request_submitted_confirmation_email_custom_intro', NULL);      

  
  //$body .=
  //  t("You can view your submission at any time by visiting !url\n" .
  //    "\n",
  //    $body_variables);

  
  $body .=
    t(
      "SUBMITTED CONTENT\n" .
      "====================\n" .
      "\n",
      $body_variables);

  $body .=
    t("Created on: !created\n" .
      "\n",
      $body_variables);

  // for some reason the \n is not printing unless the project settings are
  // listed separately
  $body .= t("Project Settings\n", $body_variables);
   
  $body .= t("--------------------\n", $body_variables);
      
  $body .= t("Project Title: !series_title", $body_variables);

  $body .= "\n\n";  

  $body .= t("Project Type: !project_type\n", $body_variables);

  $body .= t("Series Frequency: !series_request_frequency\n", 
      $body_variables);
      
  $body .= t("Estimated Completion: !est_completion_date",
      $body_variables);

  $body .= "\n\n";      
      
  $body .= t("Keep Current Schedule?: !keep_current_schedule\n\n",
      $body_variables);

  $body .=
    t(      
      "Public Contact Information\n" .
      "------------------------------\n" .
      "Producer Name: !pub_producer_name\n" .
      "Contact Phone: !pub_contact_phone\n" .
      "Email: !pub_contact_email\n" .
      "Website: !pub_website \n" .
      "\n",
      $body_variables);

  $body .=
    t(       
      "Show Settings\n" .
      "----------------\n" .
      "Languages: !pbcore_languages\n" .
      "Genres: !pbcore_genres\n" .
      "Short Description: !short_description\n" .
      "Description: !description_body\n" .
      "Live: !live\n",
      $body_variables); 
  
  if ($series_request['live'] == 'Yes') {
    $body .=
      t( 
        "Call-In: !call_in\n",
        $body_variables);
  }
  else {
    $body .=
      t(       
        "Adult Content: !objectionable_content\n",
        $body_variables);
  }  
   
  $body .=
    t(       
      "Closed Captions: !closed_captions\n" .
      "Filler: !filler\n" .
      "Filler end date: !filler_end_date\n" .
      "Runtime: !runtime_default\n" .
      "Playback Preferences: !playback_preferences\n" .
      "\n",
      $body_variables); 

    $body .= 
    t(       
      "\n\nEND SUBMITTED CONTENT\n" .
      "====================\n" .
      "\n",
      $body_variables);

  $body .= variable_get('series_request_submitted_confirmation_email_closing', NULL);
  //dpm($body, 'body');
  
  return $body;
}
 

////////////////////////////////////////
// Email confirmation to producers of approved and scheduled series
function cm_series_request_schedule_confirmation_email(&$node, $context = array()) {
  
  _cm_series_request_schedule_confirmation_email($node);
}

function _cm_series_request_schedule_confirmation_email(&$node, &$context) {
  $series_request = cm_series_request_get_series_request($node);
  if (!isset($series_request)) {
    return;     // skip to next record
  }
  
//dpm($series_request, 'series_request');  
//dpm($node, 'node_confirm');

  if (isset($_SESSION['pcm_preview_email']) && $_SESSION['pcm_preview_email']){
    $to = $_SESSION['pcm_preview_email'];
    $series_request['bcc'] = "";    
  }
  else {
    $to = $series_request['producers_email'];
    $series_request['bcc'] = 
      'frontdesk@opensignalpdx.org,playback@opensignalpdx.org';
  }
  
  // prep and send the email
  $module = "cm_series_request";
  $key = "series request schedule confirmaion";

  $language = language_default();
  $series_request['langcode'] = $language->language;
  $params = $series_request;
  $from = variable_get('site_mail');
  if (!$from) {
    drupal_set_message("Error: Site wide email address is not set: ".
                      "_cm_series_request_schedule_confirmation_email", 'error');
  }
  $send = TRUE;
  
  // no need to check for failure - drupal_mail $message['result'] contains
  // the success indicator of the email, failure being already written to the
  // watchdog; success means nothing more than the message being accepted at
  // php-level, which still doesn't guarantee it to be delivered
  // calls cm_series_request_mail
  $mail = drupal_mail($module, $key, $to, $language, $params, $from, $send);
  
  if (!$mail['result']) {
    drupal_set_message("Error: Message failed to send to " . 
        $series_request['producers_name'] .
        " for project: " . $series_request['project_title'] .
        " (" . $series_request['project_nid'] . ")", 'error');
  }      
}

//------------------------------------------------------------------------------
// DRUPAL_MAIL
//------------------------------------------------------------------------------
/**
 * 
 * Implements hook_mail - invoked by drupal_mail()
 * 
 */
function cm_series_request_mail($key, &$message, $params) {
  $options = array(
    'langcode' => $params['langcode'],
  );
  
  // mime mail module makes all emails HTML so this will 
  // ensure we get plain text emails
  $message['params']['plain'] = TRUE;
  
  switch($key) {   
    // confirmation sent for scheduled series requests
    case 'series request schedule confirmaion':

      if (isset($params['bcc']) && $params['bcc']) {
	$message['headers']['Bcc'] = $params['bcc'];
      }

      $subject_variables = array(
          '!project_title' => $params['project_title'], 
      );
      $subject = t("!project_title : Airing Schedule Confirmed", $subject_variables, $options);
      $message['subject'] = t($subject, $subject_variables, $options);
      
      $body_variables = array(
          '!project_title' => $params['project_title'], 
          '!season_start' => $params['season_start_string'],
          '!season_end' => $params['season_end_string'],
          '!season_title' => $params['season_title'],
          '!name' => $params['producers_name'],
          '!url' => "www.opensignalpdx.org",
      );

      $message['body'] = array();      
      $txt = variable_get('series_request_schedule_confirmation_email_intro', NULL);
      $message['body'][] = t($txt, $body_variables, $options);
            
      $txt = "The !season_title Season is !season_start to !season_end.\r\n\r\n";
      $message['body'][] = t($txt, $body_variables, $options);
     
      $message['body'][] = "\n Channel \t\t\t Date \t\t\t Time \t\t\t\t Show"; 

      $is_first = TRUE;
      foreach ($params['airings'] as $airings) {
	$airing_variables = 
	  array(
		'!airing_id' => $airings['airing_id'],
		'!date' => date('m-d-Y', strtotime($airings['date_start'])),
		'!time' => date('h:i a', strtotime($airings['date_start'])),
		'!channel' => $airings['channel'],
		'!show_title' => $airings['show_title'],
		);

	$is_premiere = $airings['is_premiere'];
	if ($is_premiere == 'Y'){
	  $message['body'][] = 
	    t("---------------------------------------------" . 
	      "----------------------------------------------" .
	      "----------------------------------------------", 
        $airing_variables, $options);
	}
          
	$message['body'][] = 
	  t("      " .
	    "!channel\t\t\t" .
	    "!date\t" .
	    "!time\t" .
	    "!show_title\t", 
	    $airing_variables, $options);
      }

      if ($params['other_airings']) {
	$message['body'][] = 
	"\n\n-------------------OTHER SCHEDULED AIRINGS-------------------\n";
      }
      foreach ($params['other_airings'] as $airings) {
	$airing_variables = 
	  array(
		'!airing_id' => $airings['airing_id'],
		'!date' => date('m-d-Y', strtotime($airings['date_start'])),
		'!time' => date('h:i a', strtotime($airings['date_start'])),
		'!channel' => $airings['channel'],
		'!show_title' => $airings['show_title'],
		);

	$message['body'][] = 
	  t("      " .
	    "!channel\t\t\t" .
	    "!date\t\t" .
	    "!time\t" .
	    "!show_title\t", 
	    $airing_variables, $options);
      }
      
      $txt = variable_get('series_request_schedule_confirmation_email_closing', NULL);   
      $message['body'][] = t($txt, $body_variables, $options);
      
      break;   //end of 'series request schedule confirmaion'
  }
  return $message;
}
