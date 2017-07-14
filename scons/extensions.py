
def get_sources_and_includes(env, node):
	sources = set()
	includes = set()
	get_sources_and_includes_recursive(env, node, sources, includes)
	return sources, includes

def get_sources_and_includes_recursive(env, node, sources, includes):
	if isinstance(node, basestring):
		node = env.File(node)
	if node.implicit is None or node.sources is None:
		node.scan()
	if node.sources is not None:
		for src in node.sources:
			sources.add(src)
			get_sources_and_includes_recursive(env, src, sources, includes)
	if node.implicit is not None:
		for inc in node.implicit:
			includes.add(inc)
			get_sources_and_includes_recursive(env, inc, sources, includes)