STUID = 222023321102085# 学号
STUNAME = 冯辞羽# 姓名

# DO NOT modify the following code!!!

TRACER = tracer-$(STUID)-$(STUNAME)
GITFLAGS = -q --author='$(TRACER) <tracer@gkd>' --no-verify --allow-empty

WORK_BRANCH = $(shell git rev-parse --abbrev-ref HEAD)
WORK_INDEX = $(LOCK_DIR)/index.$(WORK_BRANCH)
TRACER_BRANCH = $(TRACER)


# prototype: git_soft_checkout(branch)
define git_soft_checkout
	git checkout --detach -q && git reset --soft $(1) -q -- && git checkout $(1) -q --
endef

# prototype: git_commit(msg)
define git_commit
	@pwd
	-@flock $(LOCK_DIR) $(MAKE) -C . .git_commit MSG='$(1)'
	-@sync
endef

.git_commit:
	-@while (test -e $(LOCK_DIR)/index.lock); do sleep 0.1; done;               `# wait for other git instances`
	-@git branch $(TRACER_BRANCH) -q 2>/dev/null || true                 `# create tracer branch if not existent`
	-@cp -a $(LOCK_DIR)/index $(WORK_INDEX)                                     `# backup git index`
	-@$(call git_soft_checkout, $(TRACER_BRANCH))                        `# switch to tracer branch`
	-@git add . -A --ignore-errors                                       `# add files to commit`
	-@(echo "> $(MSG)" && echo $(STUID) $(STUNAME) && uname -a && uptime `# generate commit msg`) \
	                | git commit -F - $(GITFLAGS)                        `# commit changes in tracer branch`
	-@$(call git_soft_checkout, $(WORK_BRANCH))                          `# switch to work branch`
	-@mv $(WORK_INDEX) $(LOCK_DIR)/index                                        `# restore git index`

.clean_index:
	rm -f $(WORK_INDEX)

.PHONY: .git_commit .clean_index _default
