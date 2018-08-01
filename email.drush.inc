<?php

/**
 * Implements hook_drush_command().
 */
function email_drush_command() {
  
  $items['email'] = array(
    'description' => 'Sends emails via Drupal\'s internal mailing system',
    'aliases' => array(),
    'arguments' => array(
    ),
    'options' => array(
      'html' => 'Specify this as any value to send as HTML instead of plain text',
      'subject' => 'The subject line to send',
      'to' => 'The email address(es) to send the email to',
      'cc' => 'Carbon-copy email(s)',
      'body' => 'The path to the file where the email body is stored',
      'attachment' => 'The path to the file where the attachment is',
    ),
    'examples' => array(
      'drush email --to=test@example.com --subject="Test email subject" --body=/var/www/input.txt' => 'Sends an email to test@emample.com with the subject "Test email subject" and body with the contents of input.txt',
      'drush email --to=test@example.com --subject="Test email subject" --body=/var/www/input.txt --cc=other@example.com' => 'Sends an email to test@emample.com and CCs other@example.com with the subject "Test email subject" and body with the contents of input.txt',
      'drush email --to=test@example.com --subject="Test email subject" --body=/var/www/input.txt --attachment=/var/www/attach.bin' => 'Sends an email to test@emample.com with the subject "Test email subject" and body with the contents of input.txt and an attachment, attach.bin',
    ),
  );
  
  return $items;
}

/**
 * Callback for the drush-demo-command command
 */
function drush_email() {
  $html = drush_get_option('html', FALSE);
  $subject = drush_get_option('subject', '');
  $to = drush_get_option('to', '');
  $cc = drush_get_option('cc', '');
  $attachment = drush_get_option('attachment', '');
  $body_file = drush_get_option('body', '');
  
  // verification
  
  if (empty($to)) {
    return drush_set_error('Please specify a recipient via the --to option');
  }
  
  if (empty($subject)) {
    if (!drush_confirm('Are you sure you want to send without a subject line?')) {
      echo "Povide a subject line via the --subject option\n";
      return drush_user_abort();
    }
  }
  
  if (empty($body_file)) {
    if (!drush_confirm('Are you sure you want to send without a body?')) {
      echo "Provide a file to pull the body from via the --body option\n";
      return drush_user_abort();
    }
  }
  
  //where $filepathname should contain the path to the file and $filename should contain the name of the file.
    
  $module = 'drush';
  $key = 'email';
  
  $language = language_default();
  $params = array();
  $site_email = variable_get('site_mail', '');
  $send = FALSE; //
  $message = drupal_mail($module, $key, $to, $language, $params, $site_email, $send);
  $message['subject'] = $subject;
  
  // Always send HTML email
  $message['headers'] = array(
    'Content-Type' => 'text/html',
  );
  
  // Send carbon-copy if specified.
  if (!empty($cc)) {
    $message['headers']['Cc'] = $cc;
  }
  
  // Make sure the body file exists.
  if (!file_exists($body_file)) {
    return drush_set_error('The body file does not exist');
  }
  
  // Make sure the body isn't empty
  $body = file_get_contents($body_file);
  if (empty($body)) {
    if (!drush_confirm('The body file is empty, did you want to send without a body?')) {
      return drush_user_abort();
    }
  }
  
  // If the input body file isn't HTML, let's convert those line breaks to <br>
  if (empty($html)) {
    // Convert HTML tag <> to special characters like &lt; and &gt;
    $body = htmlspecialchars($body);
    $body = _filter_autop($body);
  }
  
  $message['body'] = $body;
  
  if (!empty($attachment)) {
    
    if (!file_exists($attachment)) {
      return drush_set_error('File does not exist');
    }
    
    // Get the file contents to send, and determine the mime type.
    $contents = file_get_contents($attachment);
      
    $finfo = finfo_open(FILEINFO_MIME_TYPE);
    
    $attachment = array(
      'filecontent' => $contents,
      'filename' => basename($attachment),
      'filemime' => finfo_file($finfo, $attachment),
    );

    $message['params']['attachments'][] = $attachment;
  }
  
  // Retrieve the responsible implementation for this message.
  $system = drupal_mail_system($module, $key);
  // Format the message body.
  $message = $system->format($message);
  
  // Send e-mail.
  $message['result'] = $system->mail($message);

}