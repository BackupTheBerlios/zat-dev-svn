COMMENT=	"Zero Attitude Toolchain"

ZAT=		0.1
SNAPSHOT=	20050324
VERSION=	${ZAT}-${SNAPSHOT}
DISTNAME=	zat-${VERSION}
CATEGORIES=	devel
MASTER_SITES=	http://zat.sourceforge.net/files/

HOMEPAGE=	http://zat.sourceforge.net/

MAINTAINER=	hangy <hangy@irssi.ru>

# GPL
PERMIT_PACKAGE_CDROM=	Yes
PERMIT_PACKAGE_FTP=	Yes
PERMIT_DISTFILES_CDROM=	Yes
PERMIT_DISTFILES_FTP=	Yes

WRKSRC=		${WRKDIR}/zat-${ZAT}

USE_GMAKE=	Yes

MAKE_FILE=	GNUmakefile

CONFIGURE_STYLE=simple noman

pre-patch:
	@ln -s ${WRKDIR}/zat-${ZAT} ${WRKDIR}/${DISTNAME}

.include <bsd.port.mk>
