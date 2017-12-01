#!/usr/bin/perl -w
#
# This script reads "nanovg_gl.h", adds "QNANO_GLFWRAPPER" prefix
# into every OpenGL command and outputs "nanovg_gl_wrapped.h".
#
# QNANO_GLFWRAPPER can then be replaced with a define, in QNanoPainter
# case "glf->" which is QOpenGLFunctions so Qt can do GL resolving.

use strict;

# Array of all OpenGL commands
# used in NanoVG
my @gl_commands = (
"glCreateProgram",
"glCreateShader",
"glShaderSource",
"glCompileShader",
"glGetShaderiv",
"glAttachShader",
"glBindAttribLocation",
"glLinkProgram",
"glGetProgramiv",
"glDeleteProgram",
"glDeleteShader",
"glGetUniformLocation",
"glGetUniformBlockIndex",
"glGenVertexArrays",
"glGenBuffers",
"glUniformBlockBinding",
"glGetIntegerv",
"glFinish",
"glGenTextures",
"glPixelStorei",
"glTexParameteri",
"glTexImage2D",
"glGenerateMipmap",
"glTexSubImage2D",
"glUniform4fv",
"glEnable",
"glColorMask",
"glDisable",
"glDrawArrays",
"glStencilOp",
"glUseProgram",
"glCullFace",
"glFrontFace",
"glStencilMask",
"glStencilFunc",
"glActiveTexture",
"glBindTexture",
"glBindBuffer",
"glBufferData",
"glBindVertexArray",
"glVertexAttribPointer",
"glUniform1i",
"glDeleteBuffers",
"glDeleteVertexArrays",
"glDeleteTextures",
"glGetProgramInfoLog",
"glGetShaderInfoLog",
"glBlendFuncSeparate",
"glGetError"
);

open(FILE, "<libqnanopainter/nanovg/nanovg_gl.h") || die "File not found";
my @lines = <FILE>;
close(FILE);

my @newlines;
foreach(@lines) {
  foreach my $command (@gl_commands) {
    $_ =~ s/$command/QNANO_GLFWRAPPER $command/g;
  }
  push(@newlines,$_);
}

open(FILE, "> libqnanopainter/nanovg/nanovg_gl_wrapped.h") || die "File not found";
print FILE @newlines;
close(FILE);