<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:import href="/home_local/usuario/docbook-xsl-ns-1.74.0/fo/docbook.xsl"/>
<xsl:param name="html.stylesheet" select="'corpstyle.css'"/>
<xsl:param name="admon.graphics" select="1"/>
<xsl:param name="paper.type" select="'A4'"/> 
<xsl:attribute-set name="monospace.verbatim.properties">
    <xsl:attribute name="wrap-option">wrap</xsl:attribute>
    <xsl:attribute name="hyphenation-character">\</xsl:attribute>
</xsl:attribute-set>
<xsl:param name="shade.verbatim" select="1"/>
<xsl:attribute-set name="shade.verbatim.style">
  <xsl:attribute name="background-color">#E0E0E0</xsl:attribute>
  <xsl:attribute name="border-width">0.5pt</xsl:attribute>
  <xsl:attribute name="border-style">solid</xsl:attribute>
  <xsl:attribute name="border-color">#575757</xsl:attribute>
  <xsl:attribute name="padding">3pt</xsl:attribute>
</xsl:attribute-set>
</xsl:stylesheet>
