Name:       org.tizen.voice-setting
Summary:    Voice Setting Application
Version:    0.0.1
Release:    1
Group:      Graphics & UI Framework/Voice Framework
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

%if "%{?tizen_profile_name}" == "tv"
ExcludeArch: %{arm} %ix86 x86_64
%endif

BuildRequires:  gettext-tools
BuildRequires:  pkgconfig(capi-appfw-application)
BuildRequires:  pkgconfig(appcore-efl)
BuildRequires:  pkgconfig(stt-setting)
BuildRequires:  pkgconfig(tts-setting)
BuildRequires:  pkgconfig(voice-control-setting)
BuildRequires:  pkgconfig(elementary)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(vconf-internal-keys)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(pkgmgr-info)
BuildRequires:  cmake
BuildRequires:  efl-extension-devel

%description
Voice Setting Application.

%prep
%setup -q -n %{name}-%{version}

%build
export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"

%if "%{?tizen_profile_name}" == "mobile"
CFLAGS+=" -D_MOBILE";
CXXFLAGS+=" -D_MOBILE";
%endif

%if "%{?tizen_profile_name}" == "wearable"
CFLAGS+=" -D_WEARABLE";
CXXFLAGS+=" -D_WEARABLE";
%endif

rm -rf CMakeFiles
rm -rf CMakeCache.txt
cmake . -DCMAKE_INSTALL_PREFIX=/usr
make %{?jobs:-j%jobs}

%post
/sbin/ldconfig

mkdir -p /usr/apps/org.tizen.voice-setting/bin
mkdir -p /usr/apps/org.tizen.voice-setting/res

%postun -p /sbin/ldconfig

%install
rm -rf %{buildroot}

%make_install
mkdir -p %{buildroot}/usr/share/license

%files
%manifest org.tizen.voice-setting.manifest
%defattr(-,root,root,-)
/usr/apps/org.tizen.voice-setting/bin/*
/usr/apps/org.tizen.voice-setting/res/*
/usr/share/license/%{name}
/usr/share/packages/org.tizen.voice-setting.xml
