#include "preferences.hh"
#include "keyboardstate.hh"

Preferences::Preferences( QWidget * parent, Config::Preferences const & p ):
  QDialog( parent )
{
  ui.setupUi( this );

  connect( ui.enableScanPopup, SIGNAL( toggled( bool ) ),
           this, SLOT( enableScanPopupToggled( bool ) ) );

  connect( ui.enableScanPopupModifiers, SIGNAL( toggled( bool ) ),
           this, SLOT( enableScanPopupModifiersToggled( bool ) ) );

  connect( ui.altKey, SIGNAL( clicked( bool ) ),
           this, SLOT( wholeAltClicked( bool ) ) );
  connect( ui.ctrlKey, SIGNAL( clicked( bool ) ),
           this, SLOT( wholeCtrlClicked( bool ) ) );
  connect( ui.shiftKey, SIGNAL( clicked( bool ) ),
           this, SLOT( wholeShiftClicked( bool ) ) );

  connect( ui.leftAlt, SIGNAL( clicked( bool ) ),
           this, SLOT( sideAltClicked( bool ) ) );
  connect( ui.rightAlt, SIGNAL( clicked( bool ) ),
           this, SLOT( sideAltClicked( bool ) ) );
  connect( ui.leftCtrl, SIGNAL( clicked( bool ) ),
           this, SLOT( sideCtrlClicked( bool ) ) );
  connect( ui.rightCtrl, SIGNAL( clicked( bool ) ),
           this, SLOT( sideCtrlClicked( bool ) ) );
  connect( ui.leftShift, SIGNAL( clicked( bool ) ),
           this, SLOT( sideShiftClicked( bool ) ) );
  connect( ui.rightShift, SIGNAL( clicked( bool ) ),
           this, SLOT( sideShiftClicked( bool ) ) );

  // Load values into form

  ui.enableTrayIcon->setChecked( p.enableTrayIcon );
  ui.startToTray->setChecked( p.startToTray );
  ui.closeToTray->setChecked( p.closeToTray );
  ui.enableScanPopup->setChecked( p.enableScanPopup );
  ui.startWithScanPopupOn->setChecked( p.startWithScanPopupOn );
  ui.enableScanPopupModifiers->setChecked( p.enableScanPopupModifiers );

  ui.altKey->setChecked( p.scanPopupModifiers & KeyboardState::Alt );
  ui.ctrlKey->setChecked( p.scanPopupModifiers & KeyboardState::Ctrl );
  ui.shiftKey->setChecked( p.scanPopupModifiers & KeyboardState::Shift );
  ui.winKey->setChecked( p.scanPopupModifiers & KeyboardState::Win );
  ui.leftAlt->setChecked( p.scanPopupModifiers & KeyboardState::LeftAlt );
  ui.rightAlt->setChecked( p.scanPopupModifiers & KeyboardState::RightAlt );
  ui.leftCtrl->setChecked( p.scanPopupModifiers & KeyboardState::LeftCtrl );
  ui.rightCtrl->setChecked( p.scanPopupModifiers & KeyboardState::RightCtrl );
  ui.leftShift->setChecked( p.scanPopupModifiers & KeyboardState::LeftShift );
  ui.rightShift->setChecked( p.scanPopupModifiers & KeyboardState::RightShift );

  // Different platforms have different keys available

#ifdef Q_OS_WIN32
  ui.winKey->hide();
#else
  ui.leftAlt->hide();
  ui.rightAlt->hide();
  ui.leftCtrl->hide();
  ui.rightCtrl->hide();
  ui.leftShift->hide();
  ui.rightShift->hide();
#endif

  // Sound

  ui.pronounceOnLoadPopup->setEnabled( p.enableScanPopup );

  ui.pronounceOnLoadMain->setChecked( p.pronounceOnLoadMain );
  ui.pronounceOnLoadPopup->setChecked( p.pronounceOnLoadPopup );
  ui.audioPlaybackProgram->setText( p.audioPlaybackProgram );

#ifdef Q_OS_WIN32
  ui.audioPlaybackProgram->hide();
#endif

  // Proxy server

  ui.useProxyServer->setChecked( p.proxyServer.enabled );

  ui.proxyType->addItem( "SOCKS5" );
  ui.proxyType->addItem( "HTTP Transp." );
  ui.proxyType->addItem( "HTTP Caching" );

  ui.proxyType->setCurrentIndex( p.proxyServer.type );

  ui.proxyHost->setText( p.proxyServer.host );
  ui.proxyPort->setValue( p.proxyServer.port );

  ui.proxyUser->setText( p.proxyServer.user );
  ui.proxyPassword->setText( p.proxyServer.password );
}

Config::Preferences Preferences::getPreferences()
{
  Config::Preferences p;

  p.enableTrayIcon = ui.enableTrayIcon->isChecked();
  p.startToTray = ui.startToTray->isChecked();
  p.closeToTray = ui.closeToTray->isChecked();
  p.enableScanPopup = ui.enableScanPopup->isChecked();
  p.startWithScanPopupOn = ui.startWithScanPopupOn->isChecked();
  p.enableScanPopupModifiers = ui.enableScanPopupModifiers->isChecked();

  p.scanPopupModifiers += ui.altKey->isChecked() ? KeyboardState::Alt : 0;
  p.scanPopupModifiers += ui.ctrlKey->isChecked() ? KeyboardState::Ctrl: 0;
  p.scanPopupModifiers += ui.shiftKey->isChecked() ? KeyboardState::Shift: 0;
  p.scanPopupModifiers += ui.winKey->isChecked() ? KeyboardState::Win: 0;
  p.scanPopupModifiers += ui.leftAlt->isChecked() ? KeyboardState::LeftAlt: 0;
  p.scanPopupModifiers += ui.rightAlt->isChecked() ? KeyboardState::RightAlt: 0;
  p.scanPopupModifiers += ui.leftCtrl->isChecked() ? KeyboardState::LeftCtrl: 0;
  p.scanPopupModifiers += ui.rightCtrl->isChecked() ? KeyboardState::RightCtrl: 0;
  p.scanPopupModifiers += ui.leftShift->isChecked() ? KeyboardState::LeftShift: 0;
  p.scanPopupModifiers += ui.rightShift->isChecked() ? KeyboardState::RightShift: 0;

  p.pronounceOnLoadMain = ui.pronounceOnLoadMain->isChecked();
  p.pronounceOnLoadPopup = ui.pronounceOnLoadPopup->isChecked();
  p.audioPlaybackProgram = ui.audioPlaybackProgram->text();

  p.proxyServer.enabled = ui.useProxyServer->isChecked();

  p.proxyServer.type = ( Config::ProxyServer::Type ) ui.proxyType->currentIndex();

  p.proxyServer.host = ui.proxyHost->text();
  p.proxyServer.port = ( unsigned ) ui.proxyPort->value();

  p.proxyServer.user = ui.proxyUser->text();
  p.proxyServer.password = ui.proxyPassword->text();

  return p;
}

void Preferences::enableScanPopupToggled( bool b )
{
  ui.scanPopupModifiers->setEnabled( b && ui.enableScanPopupModifiers->isChecked() );
  ui.pronounceOnLoadPopup->setEnabled( b );
}

void Preferences::enableScanPopupModifiersToggled( bool b )
{
  ui.scanPopupModifiers->setEnabled( b && ui.enableScanPopup->isChecked() );
}

void Preferences::wholeAltClicked( bool b )
{
  if ( b )
  {
    ui.leftAlt->setChecked( false );
    ui.rightAlt->setChecked( false );
  }
}

void Preferences::wholeCtrlClicked( bool b )
{
  if ( b )
  {
    ui.leftCtrl->setChecked( false );
    ui.rightCtrl->setChecked( false );
  }
}

void Preferences::wholeShiftClicked( bool b )
{
  if ( b )
  {
    ui.leftShift->setChecked( false );
    ui.rightShift->setChecked( false );
  }
}

void Preferences::sideAltClicked( bool )
{
  if ( ui.leftAlt->isChecked() || ui.rightAlt->isChecked() )
    ui.altKey->setChecked( false );
}

void Preferences::sideCtrlClicked( bool )
{
  if ( ui.leftCtrl->isChecked() || ui.rightCtrl->isChecked() )
    ui.ctrlKey->setChecked( false );
}

void Preferences::sideShiftClicked( bool )
{
  if ( ui.leftShift->isChecked() || ui.rightShift->isChecked() )
    ui.shiftKey->setChecked( false );
}
