# Introduction
This module provides a way for a drupal website or subsite to use their individual mail settings to send email through a drush command.
The following is what it can do:
- Send email via the drupal email system
- Put attachments on emails

# Installation Instructions
All that is required is to place this file in ~/.drush for whatever environment you are using and clear the drush using `drush cc drush`. Then you can use it immediately with these options:
`drush email <options>`:
- `html` -- Specify this as any value to send as HTML instead of plain text.
- `subject` -- The subject line to send
- `to` -- The email address(es) to send the email to.
- `cc` -- Carbon-copy email(s), comma separated.
- `body` -- The path to the file where the email body is stored.
- `attachment` -- The path to the file where the attachment is.

# Example usage

### The following is also listed as suggestions when you try to use the command "drush email"

Sends an email to test@emample.com with the subject "Test email subject" and body with the contents of input.txt:
`drush email --to=test@example.com --subject="Test email subject" --body=/var/www/input.txt'`

Sends an email to test@emample.com and CCs other@example.com with the subject "Test email subject" and body with the contents of input.txt:
`drush email --to=test@example.com --subject="Test email subject" --body=/var/www/input.txt --cc=other@example.com'`

Sends an email to test@emample.com with the subject "Test email subject" and body with the contents of input.txt and an attachment, attachment.bin:
`drush email --to=test@example.com --subject="Test email subject" --body=/var/www/input.txt --attachment=/var/www/attachment.bin'`
